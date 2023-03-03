/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/ztest.h>
#include <stdio.h>
#include <string.h>
#include <nrf_modem_at.h>
#include <modem/nrf_modem_lib.h>
#include <modem/at_custom_cmd.h>
#include <nrf_errno.h>

static char response[64];

/* AT filter function declarations. */
static int at_cmd_callback_cmd1(char *buf, size_t len, char *at_cmd)
{
	zassert_mem_equal("AT+CMD1", at_cmd, strlen("AT+CMD1"), NULL);
	return at_custom_cmd_respond(buf, len, "\r\n+CMD1: OK\r\n");
}

static int at_cmd_callback_cmd2(char *buf, size_t len, char *at_cmd)
{
	zassert_mem_equal("AT+CMD2", at_cmd, strlen("AT+CMD2"), NULL);
	return at_custom_cmd_respond(buf, len,
	"\r\n+CPMS: \"TA\",%d,%d,\"TA\",%d,%d\r\nOK\r\n", 0, 3, 0, 3);
}

/* AT filter list
 * Including all comands the filter should check for and function ptr
 * to functions to be called on detection.
 */

AT_CUSTOM_CMD(CMD1, "AT+CMD1", at_cmd_callback_cmd1);
AT_CUSTOM_CMD(CMD2, "AT+CMD2", at_cmd_callback_cmd2);

static void test_at_custom_cmd_response(void)
{
	int err;

	err = nrf_modem_at_cmd(response, sizeof(response), "AT+CMD1");
	zassert_equal(0, err, "nrf_modem_at_cmd failed, error: %d", err);
	zassert_mem_equal(response, "\r\n+CMD1: OK", strlen("\r\n+CMD1: OK"), NULL);

	err = nrf_modem_at_cmd(response, sizeof(response), "AT+CMD2");
	zassert_equal(0, err, "nrf_modem_at_cmd failed, error: %d", err);
	zassert_mem_equal(response, "\r\n+CPMS: \"TA\",0,3,\"TA\",0,3",
			strlen("\r\n+CPMS: \"TA\",0,3,\"TA\",0,3"), NULL);
}

static void test_at_custom_cmd_buffer_size(void)
{
	int err;

	err = nrf_modem_at_cmd(response, 1, "AT+CMD1");
	zassert_equal(-NRF_E2BIG, err, "nrf_modem_at_cmd failed, error: %d", err);
}

static void test_at_custom_cmd_command_fault_on_NULL_buffer(void)
{
	int err;

	err = nrf_modem_at_cmd(NULL, 100, "AT+CMD1");
	zassert_equal(-NRF_EFAULT, err, "nrf_modem_at_cmd failed, error: %d", err);
}

void test_main(void)
{
	ztest_test_suite(at_cmd_filter,
		ztest_unit_test(test_at_custom_cmd_response),
		ztest_unit_test(test_at_custom_cmd_buffer_size),
		ztest_unit_test(test_at_custom_cmd_command_fault_on_NULL_buffer)
	);

	ztest_run_test_suite(at_cmd_filter);
}
