#include <err.h>
#include <stdio.h>
#include <string.h>

/* OP-TEE TEE client API */
#include <tee_client_api.h>

/* Contains the UUID of the TA */
#include <calculator_ta.h>

int main(void)
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = TA_CALCULATOR_UUID;
	uint32_t err_origin;

	printf("Initialize context\n");
	/* Creates new context for connecting to TEE */
	res = TEEC_InitializeContext(NULL, &ctx);
	/* if res!=0x0, errx exits with error code in first parameter and 
	 * printf style string as error message.
	 */
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
	
	printf("Open session\n");
	/* Opens a new session, connection method is defined as 
	 * LOGIN_PUBLIC e.g., no login data is provided. No data buffers
	 * are exchanged with the TA, operation cannot be cancelled.
	 */ 
	res = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC,
			NULL, NULL, &err_origin);
	if (res !=TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
				res, err_origin);

	/*	
	 *Clear the TEEC_Operation struct (to be able to start the 
	 * TA, the started field of the Operation struct must be zeroed.)
	 */
	memset(&op, 0, sizeof(op));

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT,TEEC_VALUE_INPUT,
			TEEC_NONE,TEEC_NONE);

	op.params[0].value.a = 15;
	op.params[1].value.a = 4; //unsigned integer

	printf("Invoking the TA\n");

	/*
	 * Call the TA functionality (macros are defined in TA header file)
	 */
	res = TEEC_InvokeCommand(&sess, TA_CALCULATOR_SUBTRACT, &op, &err_origin);

	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
		res, err_origin);

	printf("Operation result in the CA: %d\n", op.params[0].value.a);

	printf("close session\n");
	/* 
	 * Done with the TA, do cleanup
	 */
	TEEC_CloseSession(&sess);
	
	printf("finalizing context\n");
	TEEC_FinalizeContext(&ctx);

	return 0;
}






