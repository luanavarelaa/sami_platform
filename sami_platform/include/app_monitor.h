#ifndef APP_MONITOR_H
#define APP_MONITOR_H

void app_monitor_init(void);

void app_monitor_task(void);

void app_monitor_check_alerts(int current_time);

void app_monitor_evaluate_schedule(int current_time);

void app_monitor_check_box(void);

void app_monitor_send_messages(void);

#endif