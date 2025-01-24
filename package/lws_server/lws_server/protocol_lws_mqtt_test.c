#if !defined (LWS_PLUGIN_STATIC)
#define LWS_DLL
#define LWS_INTERNAL
#include <libwebsockets.h>
#endif

#include <sys/stat.h>
#include <string.h>
#include <cjson/cJSON.h>

#define MQTT_CONFIGURATION_FILE "/etc/websocket/mqtt_conf.json"

#define LWS_MSG_TYPE_SIMPLE_TEXT 		"LWS_MSG_TYPE_SIMPLE_TEXT"
#define LWS_MSG_TYPE_MQTT_GET_CONFIG 	"LWS_MSG_TYPE_MQTT_GET_CONFIG"
#define LWS_MSG_TYPE_MQTT_PUBLISH_EVENT "LWS_MSG_TYPE_MQTT_PUBLISH_EVENT"

/* one of these created for each message */
struct msg {
	void *payload; /* is malloc'd */
	size_t len;
};

/* one of these is created for each client connecting to us */
struct per_session_data__mqtt_test {
	struct per_session_data__mqtt_test *pss_list;
	struct lws *wsi;
	uint32_t tail;

	unsigned int culled:1;
};

/* one of these is created for each vhost our protocol is used with */
struct per_vhost_data__mqtt_test {
	struct lws_context *context;
	struct lws_vhost *vhost;
	const struct lws_protocols *protocol;

	struct per_session_data__mqtt_test *pss_list; /* linked-list of live pss*/

	struct lws_ring *ring; /* ringbuffer holding unsent messages */
};

static void cull_lagging_clients(struct per_vhost_data__mqtt_test *vhd)
{
	uint32_t oldest_tail = lws_ring_get_oldest_tail(vhd->ring);
	struct per_session_data__mqtt_test *old_pss = NULL;
	int most = 0, before = (int)lws_ring_get_count_waiting_elements(vhd->ring,
					&oldest_tail), m;

	lws_start_foreach_llp_safe(struct per_session_data__mqtt_test **,
			      ppss, vhd->pss_list, pss_list) {

		if ((*ppss)->tail == oldest_tail) {
			old_pss = *ppss;

			lwsl_user("Killing lagging client %p\n", (*ppss)->wsi);

			lws_set_timeout((*ppss)->wsi, PENDING_TIMEOUT_LAGGING, LWS_TO_KILL_ASYNC);

			(*ppss)->culled = 1;
			lws_ll_fwd_remove(struct per_session_data__mqtt_test,
					  pss_list, (*ppss), vhd->pss_list);

			continue;

		} else {
			m = (int)lws_ring_get_count_waiting_elements(vhd->ring,
							&((*ppss)->tail));
			if (m > most)
				most = m;
		}

	} lws_end_foreach_llp_safe(ppss);

	if (!old_pss)
		return;

	lws_ring_consume_and_update_oldest_tail(vhd->ring,
		struct per_session_data__mqtt_test, &old_pss->tail, (size_t)(before - most),
		vhd->pss_list, tail, pss_list);

	lwsl_user("%s: shrunk ring from %d to %d\n", __func__, before, most);
}

static void __mqtt_test_destroy_message(void *_msg)
{
	struct msg *msg = _msg;
	if (msg->payload) {
		free(msg->payload);
		msg->payload = NULL;
		msg->len = 0;
	}
}

static int __mqtt_test_create_msg(const char *content, size_t content_len, struct msg *new_msg)
{
	lwsl_user("content = %s\n", content);
	lwsl_user("content_len = %d\n", content_len);
	lwsl_user("new_msg = %p\n", new_msg);

	if (!new_msg || !content || content_len == 0) {
		lwsl_user("Error: Invalid parameters for rsp_msg initialization\n");
		return -1;
	}

	memset(&new_msg, 0, sizeof(new_msg));
	new_msg->len = content_len;
	lwsl_user("new_msg->len = %d\n", new_msg->len);

	new_msg->payload = malloc(LWS_PRE + content_len);
	if (!new_msg->payload) {
		lwsl_user("OOM: dropping\n");
		return -1;
	}
	lwsl_user("new_msg->payload = %p\n", new_msg->payload);

	memset((char *)new_msg->payload + LWS_PRE, 0, content_len);
	memcpy((char *)new_msg->payload + LWS_PRE, content, content_len);
	lwsl_user("new_msg->payload = %s\n", (char *)new_msg->payload + LWS_PRE);

	return 0;
}

static int lws_msg_handler_mqtt_get_config(struct msg *rsp_msg)
{
	struct stat file_stat;
	FILE *file;
	char *data;
	long length;
	int status = 0;

	if (stat(MQTT_CONFIGURATION_FILE, &file_stat) != 0) {
		lwsl_user("Error getting file stats: %s\n", MQTT_CONFIGURATION_FILE);
		return -1;
	}

	file = fopen(MQTT_CONFIGURATION_FILE, "r");
	if (!file) {
		lwsl_user("Error opening file: %s\n", MQTT_CONFIGURATION_FILE);
		return -1;
	}

	length = file_stat.st_size;
	data = (char *)malloc(length + 1);
	if (!data) {
		lwsl_user("Memory allocation error\n");
		status = 1;
		goto cleanup_file;
	}


	if (length != fread(data, 1, length, file)) {
		lwsl_user("Error reading file: %s\n", MQTT_CONFIGURATION_FILE);
		status = 1;
		goto cleanup_data;
	}

	data[length] = '\0';
	lwsl_user("\n-----------------------------\n");
	lwsl_user("Read File JSON Data: \n");
	lwsl_user("-----------------------------\n");
	lwsl_user("%s\n", data);

	status = __mqtt_test_create_msg(data, length, rsp_msg);
	if (status) {
		lwsl_user("Error creating response message\n");
		goto cleanup_data;
	}

cleanup_data:
	free(data);
cleanup_file:
	fclose(file);
	return status;
}

static int lws_msg_handler_mqtt_publish_event(cJSON *data, struct msg *rsp_msg)
{
	const char *reply = "{\"status\":\"ok\"}";

    cJSON *topic = cJSON_GetObjectItem(data, "topic");
    cJSON *index = cJSON_GetObjectItem(data, "index");
    cJSON *state = cJSON_GetObjectItem(data, "state");

    if (cJSON_IsString(topic) && cJSON_IsNumber(index) && cJSON_IsString(state)) {
        lwsl_user("MQTT_PUBLISH_EVENT: topic=%s, index=%d, state=%s\n",
                topic->valuestring, index->valueint, state->valuestring);
    } else {
        lwsl_user("Error: Invalid MQTT_PUBLISH_EVENT data\n");
    }

	if (__mqtt_test_create_msg(reply, strlen(reply), rsp_msg)) {
		lwsl_user("Error creating response message\n");
		return -1;
	}

    return 0;
}


static int lws_msg_handler_simple_text(cJSON *data, struct msg *rsp_msg)
{
    cJSON *message = cJSON_GetObjectItem(data, "message");


    if (cJSON_IsString(message)) {
        lwsl_user("SIMPLE_TEXT MSG: %s\n", message->valuestring);
    } else {
        lwsl_user("Error: Invalid SIMPLE_TEXT data\n");
    }

	if (__mqtt_test_create_msg(message->valuestring, strlen(message->valuestring), rsp_msg)) {
		lwsl_user("Error creating response message\n");
		return -1;
	}

    return 0;
}

static int ws_callback_receive_parse(void *recv_data, size_t len, struct msg *rsp_msg)
{
	cJSON *cJsonRoot;
	cJSON *cJsonType;
	cJSON *cJsonData;
	char *cJsonString;
	int status;

	if (!recv_data || !len || !rsp_msg) {
		lwsl_user("Error: Invalid argument\n");
		return -1;
	}

	cJsonRoot = cJSON_Parse(recv_data);
	if (!cJsonRoot) {
		lwsl_user("Error parsing JSON\n");
		return -1;
	}

	cJsonType = cJSON_GetObjectItem(cJsonRoot, "type");
	if (!cJSON_IsString(cJsonType)) {
		lwsl_user("Error: type is not a string\n");
		cJSON_Delete(cJsonRoot);
		return -1;
	}

	cJsonData = cJSON_GetObjectItem(cJsonRoot, "data");
	if (!cJSON_IsObject(cJsonData)) {
		lwsl_user("Error: Invalid data\n");
		cJSON_Delete(cJsonRoot);
		return -1;
	}

	cJsonString = cJSON_Print(cJsonRoot);
	if (cJsonString) {
		lwsl_user("\n-----------------------------\n");
		lwsl_user("Received JSON Data: \n");
		lwsl_user("-----------------------------\n");
		lwsl_user("%s\n", cJsonString);
		lwsl_user("-----------------------------\n");
		free(cJsonString);
	}

	if (strcmp(cJsonType->valuestring, LWS_MSG_TYPE_SIMPLE_TEXT) == 0) {
		status = lws_msg_handler_simple_text(cJsonData, rsp_msg);
	} else if (strcmp(cJsonType->valuestring, LWS_MSG_TYPE_MQTT_GET_CONFIG) == 0) {
		status = lws_msg_handler_mqtt_get_config(rsp_msg);
	} else if (strcmp(cJsonType->valuestring, LWS_MSG_TYPE_MQTT_PUBLISH_EVENT) == 0) {
		status = lws_msg_handler_mqtt_publish_event(cJsonData, rsp_msg);
	} else {
		lwsl_user("Error: Unknown type %s\n", cJsonType->valuestring);
		status = 1;
	}

	lwsl_user("rsp_msg.len = %d\n", rsp_msg->len);
	lwsl_user("rsp_msg.payload = %d\n", rsp_msg->payload);

	cJSON_Delete(cJsonRoot);
	return status;
}


static int callback_mqtt_test(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	struct per_session_data__mqtt_test *pss =
			(struct per_session_data__mqtt_test *)user;
	struct per_vhost_data__mqtt_test *vhd =
			(struct per_vhost_data__mqtt_test *)
			lws_protocol_vh_priv_get(lws_get_vhost(wsi),
					lws_get_protocol(wsi));
	const struct msg *pmsg;
	struct msg amsg;
	struct msg rsp_msg;
	int n, m, status;

	switch (reason) {
	case LWS_CALLBACK_PROTOCOL_INIT:
		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__mqtt_test));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_get_protocol(wsi);
		vhd->vhost = lws_get_vhost(wsi);

		vhd->ring = lws_ring_create(sizeof(struct msg), 8,
					    __mqtt_test_destroy_message);
		if (!vhd->ring)
			return -1;
		break;

	case LWS_CALLBACK_PROTOCOL_DESTROY:
		lws_ring_destroy(vhd->ring);
		break;

	case LWS_CALLBACK_ESTABLISHED:
		lws_ll_fwd_insert(pss, pss_list, vhd->pss_list);
		pss->tail = lws_ring_get_oldest_tail(vhd->ring);
		pss->wsi = wsi;
		break;

	case LWS_CALLBACK_CLOSED:
		lwsl_user("LWS_CALLBACK_CLOSED: wsi %p\n", wsi);
		/* remove our closing pss from the list of live pss */
		lws_ll_fwd_remove(struct per_session_data__mqtt_test, pss_list,
				  pss, vhd->pss_list);
		break;

	case LWS_CALLBACK_SERVER_WRITEABLE:
		lwsl_user("LWS_CALLBACK_SERVER_WRITEABLE: wsi %p\n", wsi);
		if (pss->culled)
			break;
		pmsg = lws_ring_get_element(vhd->ring, &pss->tail);
		if (!pmsg)
			break;

		/* notice we allowed for LWS_PRE in the payload already */
		m = lws_write(wsi, ((unsigned char *)pmsg->payload) +
			      LWS_PRE, pmsg->len, LWS_WRITE_TEXT);
		if (m < (int)pmsg->len) {
			lwsl_err("ERROR %d writing to ws socket\n", m);
			return -1;
		}

		lws_ring_consume_and_update_oldest_tail(
			vhd->ring,	/* lws_ring object */
			struct per_session_data__mqtt_test, /* type of objects with tails */
			&pss->tail,	/* tail of guy doing the consuming */
			1,		/* number of payload objects being consumed */
			vhd->pss_list,	/* head of list of objects with tails */
			tail,		/* member name of tail in objects with tails */
			pss_list	/* member name of next object in objects with tails */
		);

		/* more to do for us? */
		if (lws_ring_get_element(vhd->ring, &pss->tail))
			/* come back as soon as we can write more */
			lws_callback_on_writable(pss->wsi);
		break;

	case LWS_CALLBACK_RECEIVE:
		n = (int)lws_ring_get_count_free_elements(vhd->ring);
		if (!n) {
			cull_lagging_clients(vhd);
			n = (int)lws_ring_get_count_free_elements(vhd->ring);
		}
		if (!n)
			break;

		status = ws_callback_receive_parse(in, len, &rsp_msg);
		if (status < 0) {
			lwsl_user("Error parsing received message\n");
			break;
		}

		lwsl_user("rsp_msg.len = %d\n", rsp_msg.len);
		if (rsp_msg.len != 0) {
			lwsl_user("lws_ring_insert\n");
			if (!lws_ring_insert(vhd->ring, &rsp_msg, 1)) {
				__mqtt_test_destroy_message(&rsp_msg);
				lwsl_user("dropping!\n");
				break;
			}
			lwsl_user("lws_ring_insert done\n");
			lws_start_foreach_llp(struct per_session_data__mqtt_test **,
				      ppss, vhd->pss_list) {
				lws_callback_on_writable((*ppss)->wsi);
			} lws_end_foreach_llp(ppss, pss_list);
		}

		break;

	default:
		break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_MQTT_TEST \
	{ \
		"lws-mqtt-test", \
		callback_mqtt_test, \
		sizeof(struct per_session_data__mqtt_test), \
		0, \
		0, NULL, 0 \
	}
