#ifndef SETTINGS_H
#define SETTINGS_H

#include "env.h"
#include "calibration.h"
#include "thresholds.h"
#include "pins.h"

#ifndef ENDPOINT_HOST
#define ENDPOINT_HOST "wqmp-server.vercel.app"
#endif

#ifndef ENDPOINT_ROUTE
#define ENDPOINT_ROUTE "/api/post/" DEVICE_ID
#endif

#ifndef ENDPOINT_PORT
#define ENDPOINT_PORT 443
#endif

#ifndef ALERT_HOST
#define ALERT_HOST "ntfy.sh"
#endif

#ifndef ALERT_ROUTE
#define ALERT_ROUTE "/wqmp-" DEVICE_ID
#endif

#ifndef ALERT_PORT
#define ALERT_PORT 80
#endif

#define REPORT_INTERVAL 1000*60*30
#define ALERT_COOLDOWN_INTERVAL 1000*60*60*12

#define SAMPLE_SIZE 10

#endif