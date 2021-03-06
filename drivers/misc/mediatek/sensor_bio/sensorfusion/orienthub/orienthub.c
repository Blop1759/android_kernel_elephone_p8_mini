/* linearaccityhub motion sensor driver
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <hwmsensor.h>
#include "fusion.h"
#include <orienthub.h>
#include <SCP_sensorHub.h>
#include <linux/notifier.h>
#include "scp_helper.h"

#define ORIENT_TAG                  "[orienthub] "
#define ROTVEC_FUN(f)               pr_debug(ORIENT_TAG"%s\n", __func__)
#define ROTVEC_ERR(fmt, args...)    pr_err(ORIENT_TAG"%s %d : "fmt, __func__, __LINE__, ##args)
#define ROTVEC_LOG(fmt, args...)    pr_debug(ORIENT_TAG fmt, ##args)

static struct fusion_init_info orienthub_init_info;

static int orientation_get_data(int *x, int *y, int *z, int *scalar, int *status)
{
	int err = 0;
	struct data_unit_t data;
	uint64_t time_stamp = 0;
	uint64_t time_stamp_gpt = 0;

	err = sensor_get_data_from_hub(ID_ORIENTATION, &data);
	if (err < 0) {
		ROTVEC_ERR("sensor_get_data_from_hub fail!!\n");
		return -1;
	}
	time_stamp = data.time_stamp;
	time_stamp_gpt = data.time_stamp_gpt;
	*x = data.orientation_t.azimuth;
	*y = data.orientation_t.pitch;
	*z = data.orientation_t.roll;
	*scalar = data.orientation_t.scalar;
	*status = data.orientation_t.status;
	return 0;
}

static int orientation_open_report_data(int open)
{
	return 0;
}

static int orientation_enable_nodata(int en)
{
	return sensor_enable_to_hub(ID_ORIENTATION, en);
}

static int orientation_set_delay(u64 delay)
{
#if defined CONFIG_MTK_SCP_SENSORHUB_V1
	unsigned int delayms = 0;

	delayms = delay / 1000 / 1000;
	return sensor_set_delay_to_hub(ID_ORIENTATION, delayms);
#elif defined CONFIG_NANOHUB
	return 0;
#else
	return 0;
#endif
}
static int orientation_batch(int flag, int64_t samplingPeriodNs, int64_t maxBatchReportLatencyNs)
{
	return sensor_batch_to_hub(ID_ORIENTATION, flag, samplingPeriodNs, maxBatchReportLatencyNs);
}

static int orientation_flush(void)
{
	return sensor_flush_to_hub(ID_ORIENTATION);
}
static int orientation_recv_data(struct data_unit_t *event, void *reserved)
{
	int err = 0;

	if (event->flush_action == true)
		err = orientation_flush_report();
	else
		err = orientation_data_report(event->orientation_t.azimuth, event->orientation_t.pitch,
			event->orientation_t.roll, event->orientation_t.status,
			(int64_t)(event->time_stamp + event->time_stamp_gpt));

	return err;
}
static int orienthub_local_init(void)
{
	struct fusion_control_path ctl = { 0 };
	struct fusion_data_path data = { 0 };
	int err = 0;

	ctl.open_report_data = orientation_open_report_data;
	ctl.enable_nodata = orientation_enable_nodata;
	ctl.set_delay = orientation_set_delay;
	ctl.batch = orientation_batch;
	ctl.flush = orientation_flush;
#if defined CONFIG_MTK_SCP_SENSORHUB_V1
	ctl.is_report_input_direct = true;
	ctl.is_support_batch = false;
#elif defined CONFIG_NANOHUB
	ctl.is_report_input_direct = true;
	ctl.is_support_batch = false;
#else
#endif
	err = fusion_register_control_path(&ctl, ID_ORIENTATION);
	if (err) {
		ROTVEC_ERR("register orientation control path err\n");
		goto exit;
	}

	data.get_data = orientation_get_data;
	data.vender_div = 100;
	err = fusion_register_data_path(&data, ID_ORIENTATION);
	if (err) {
		ROTVEC_ERR("register orientation data path err\n");
		goto exit;
	}
	err = SCP_sensorHub_data_registration(ID_ORIENTATION, orientation_recv_data);
	if (err < 0) {
		ROTVEC_ERR("SCP_sensorHub_data_registration failed\n");
		goto exit;
	}
	return 0;
 exit:
	return -1;
}

static int orienthub_local_uninit(void)
{
	return 0;
}

static struct fusion_init_info orienthub_init_info = {
	.name = "orientation_hub",
	.init = orienthub_local_init,
	.uninit = orienthub_local_uninit,
};

static int __init orienthub_init(void)
{
	fusion_driver_add(&orienthub_init_info, ID_ORIENTATION);
	return 0;
}

static void __exit orienthub_exit(void)
{
	ROTVEC_FUN();
}

module_init(orienthub_init);
module_exit(orienthub_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ACTIVITYHUB driver");
MODULE_AUTHOR("hongxu.zhao@mediatek.com");
