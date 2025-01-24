#include <libwebsockets.h>
#include <string.h>
#include <signal.h>

#define LWS_PLUGIN_STATIC
#include "protocol_lws_mqtt.c"

#define LWS_SERVER_PORT 8080

static struct lws_protocols protocols[] = {
	{ "http", lws_callback_http_dummy, 0, 0, 0, NULL, 0 },
	LWS_PLUGIN_PROTOCOL_MQTT,
	LWS_PROTOCOL_LIST_TERM
};

static int interrupted;

static const struct lws_http_mount mount = {
	/* .mount_next */		        NULL,		/* linked-list "next" */
	/* .mountpoint */		        "/",		/* mountpoint URL */
	/* .origin */			        "/usr/www", /* serve from dir */
	/* .def */			            "index.html",	/* default filename */
	/* .protocol */			        NULL,
	/* .cgienv */			        NULL,
	/* .extra_mimetypes */	        NULL,
	/* .interpret */		        NULL,
	/* .cgi_timeout */		        0,
	/* .cache_max_age */	        0,
	/* .auth_mask */		        0,
	/* .cache_reusable */		    0,
	/* .cache_revalidate */		    0,
	/* .cache_intermediaries */	    0,
	/* .origin_protocol */		    LWSMPRO_FILE,	/* files in a dir */
	/* .mountpoint_len */		    1,		/* char count */
	/* .basic_auth_login_file */	NULL,
};

void sigint_handler(int sig)
{
	interrupted = 1;
}

int main(int argc, const char **argv)
{
	struct lws_context_creation_info info;
	struct lws_context *context;
	const char *p;
	int n = 0, logs = LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE
			/* for LLL_ verbosity above NOTICE to be built into lws,
			 * lws must have been configured and built with
			 * -DCMAKE_BUILD_TYPE=DEBUG instead of =RELEASE */
			/* | LLL_INFO */ /* | LLL_PARSER */ /* | LLL_HEADER */
			/* | LLL_EXT */ /* | LLL_CLIENT */ /* | LLL_LATENCY */
			/* | LLL_DEBUG */;

	signal(SIGINT, sigint_handler);

	if ((p = lws_cmdline_option(argc, argv, "-d")))
		logs = atoi(p);

	lws_set_log_level(logs, NULL);
	lwsl_user("LWS minimal ws server (mqtt) | visit http://10.10.0.221:%d\n", LWS_SERVER_PORT);

	memset(&info, 0, sizeof info); /* otherwise uninitialized garbage */
	info.port = LWS_SERVER_PORT;
	info.mounts = &mount;
	info.protocols = protocols;
	info.options =
		LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;
	info.error_document_404 = "/404.html";

	context = lws_create_context(&info);
	if (context == NULL) {
		lwsl_err("lws init failed\n");
		return 1;
	}

	while (n >= 0 && !interrupted) {
		n = lws_service(context, 0);
	}

	lws_context_destroy(context);

	return 0;
}
