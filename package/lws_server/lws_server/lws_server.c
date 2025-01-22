#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <libwebsockets.h>
#include <openssl/ssl.h> // For TLS support
#include <cjson/cJSON.h>

#define PORT 8080
#define MQTT_CONFIGURATION_FILE "/etc/websocket/mqtt_conf.json"
void get_mqtt_config_handler(struct lws *wsi)
{
    FILE *file = fopen(MQTT_CONFIGURATION_FILE, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", MQTT_CONFIGURATION_FILE);
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    if (!data) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(file);
        return;
    }

    size_t read_length = fread(data, 1, length, file);
    data[read_length] = '\0';
    fclose(file);

    if (read_length != length) {
        fprintf(stderr, "Error reading file: %s\n", MQTT_CONFIGURATION_FILE);
        free(data);
        return;
    }

    printf("\n-----------------------------\n");
    printf("Read File JSON Data: \n");
    printf("-----------------------------\n");
    printf("%s\n", data);

#if 0
    if (lws_write(wsi, (unsigned char *)data, read_length, LWS_WRITE_TEXT) < 0) {
        printf("Failed to send message.\n");
    }
#endif
    free(data); // Ensure memory is freed after use
}

void simple_text_handler(struct lws *wsi, cJSON *data)
{
    cJSON *message = cJSON_GetObjectItem(data, "message");

    if (cJSON_IsString(message)) {
        printf("SIMPLE_TEXT: message=%s\n", message->valuestring);
    } else {
        fprintf(stderr, "Error: Invalid SIMPLE_TEXT data\n");
    }

    return;
}

void mqtt_publish_event_handler(struct lws *wsi, cJSON *data)
{
    cJSON *topic = cJSON_GetObjectItem(data, "topic");
    cJSON *index = cJSON_GetObjectItem(data, "index");
    cJSON *state = cJSON_GetObjectItem(data, "state");

    if (cJSON_IsString(topic) && cJSON_IsNumber(index) && cJSON_IsString(state)) {
        printf("MQTT_PUBLISH_EVENT: topic=%s, index=%d, state=%s\n",
                topic->valuestring, index->valueint, state->valuestring);
    } else {
        fprintf(stderr, "Error: Invalid MQTT_PUBLISH_EVENT data\n");
    }
    return;
}


static int ws_callback_simple(struct lws *wsi, enum lws_callback_reasons reason,
                              void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_RECEIVE: {
            cJSON *root = cJSON_Parse((const char *)in);
            if (!root) {
                fprintf(stderr, "Error parsing JSON\n");
                break;
            }

            cJSON *type = cJSON_GetObjectItem(root, "type");
            if (!cJSON_IsString(type)) {
                fprintf(stderr, "Error: type is not a string\n");
                cJSON_Delete(root);
                break;
            }

            char *json_string = cJSON_Print(root);
            if (json_string) {
                printf("\n-----------------------------\n");
                printf("Received JSON Data: \n");
                printf("-----------------------------\n");
                printf("%s\n", json_string);
                printf("-----------------------------\n");
                free(json_string);
            }

            if (strcmp(type->valuestring, "MQTT_PUBLISH_EVENT") == 0) {
                cJSON *data = cJSON_GetObjectItem(root, "data");
                if (cJSON_IsObject(data)) {
                    mqtt_publish_event_handler(wsi, data);
                } else {
                    fprintf(stderr, "Error: Invalid MQTT_PUBLISH_EVENT data\n");
                }
            } else if (strcmp(type->valuestring, "SIMPLE_TEXT") == 0) {
                cJSON *data = cJSON_GetObjectItem(root, "data");
                if (cJSON_IsObject(data)) {
                    simple_text_handler(wsi, data);
                } else {
                    fprintf(stderr, "Error: Invalid SIMPLE_TEXT data\n");
                }
            } else if (strcmp(type->valuestring, "GET_MQTT_CONFIG") == 0) {
                get_mqtt_config_handler(wsi);
            } else {
                fprintf(stderr, "Error: Unknown type %s\n", type->valuestring);
            }

            cJSON_Delete(root);

            // Echo the message back to the client (optional)
            if (lws_write(wsi, (unsigned char *)in, len, LWS_WRITE_TEXT) < 0) {
                printf("Failed to send message.\n");
            }
            break;
        }
        default:
            break;
    }
    return 0;
}

static struct lws_protocols protocols[] = {
    { "http-only", lws_callback_http_dummy, 0, 0, },
    { "WEBSOCKET_SIMPLE_PROTOCOL", ws_callback_simple, 0, 0, },
    { NULL, NULL, 0, 0 }
};

int main(void) {
    struct lws_context_creation_info info;
    struct lws_context *context;

    memset(&info, 0, sizeof(info));
    info.port = PORT;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;
    info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT; // Enable SSL global init

    context = lws_create_context(&info);
    if (context == NULL) {
        fprintf(stderr, "lws_create_context failed\n");
        return -1;
    }

    printf("Starting server...\n");
    while (1) {
        lws_service(context, 1000);
    }

    lws_context_destroy(context);

    return 0;
}
