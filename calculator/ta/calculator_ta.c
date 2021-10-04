#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <calculator_ta.h>

/*
 * TA constructor, called when the TA is created and called 
 * only once in the lifetime of the Trusted application
 * instance.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("has been called");

	return TEE_SUCCESS;	
}

/*
 * TA destructor. Function gets called before the TA instance is terminated.
 */
void TA_DestroyEntryPoint(void)
{
	DMSG("has been called");	
}

/*
 * Function is called when a client attempts to connect to the Trusted 
 * Application instance to open a new session.
 *
 * The the __maybe_unused macro tells the compiler that the variable will
 * possibly not be used and GCC will not produce a warning for this variable
 * if for example gcc is run with -Wunused-parameter.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t paramTypes,
	       	TEE_Param __maybe_unused params[4],
		void __maybe_unused **sess_ctx)
{
/* TEE_PARAM_TYPES is a macro to decode paramTypes, see documentation */
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	/* DMSG() macro is non-standard, not specifically specified in TEE
	 * Internal Api
	 */
	DMSG("has been called");

	if(paramTypes != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Tell compiler that variables are not used (works around compiler
	 * warnings)
	 */
	(void)&params;
	(void)&sess_ctx;

	/* Message from info trace level */
	IMSG("Hello from calculatorTA\n");

	return TEE_SUCCESS;
}

/* Function is called when the client closes a session and disconnects from
 * the TA instance.
 */
void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
	/* work around unused parameter warning */
	(void)&sess_ctx;

	/* message form info trace level */
	IMSG("Goodbye from calculatorTA\n");
}

/*
 * adds two values and returns them in the second parameter to the normal
 * world.
 */
static TEE_Result add_values(uint32_t paramTypes, TEE_Param params[4])
{
	uint32_t expParamTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
			TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);
	if (expParamTypes != paramTypes)
		return TEE_ERROR_BAD_PARAMETERS;
	IMSG("Calculating %d + %d\n", params[0].value.a,
			params[1].value.a);
	params[0].value.a += params[1].value.a;

	IMSG("Result: %d\n", params[0].value.a);

	return TEE_SUCCESS;
}

/*
 * multiplies two positive integers
 */
static TEE_Result multiply_values(uint32_t paramTypes, TEE_Param params[4])
{
	uint32_t expParamTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
			TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);
	if (expParamTypes != paramTypes)
		return TEE_ERROR_BAD_PARAMETERS;
	IMSG("Calculating %d * %d\n", params[0].value.a,
			params[1].value.a);
	params[0].value.a *= params[1].value.a;	
	IMSG("Result: %d\n", params[0].value.a);

	return TEE_SUCCESS;
}

/*
 * divides two positive integers if the operand is not zero
 */
static TEE_Result divide_values(uint32_t paramTypes, TEE_Param params[4])
{
	uint32_t expParamTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
			TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);
	if (expParamTypes != paramTypes)
		return TEE_ERROR_BAD_PARAMETERS;
	if (params[1].value.a == 0)
		return TEE_ERROR_BAD_PARAMETERS;
	
	IMSG("Calculating %d / %d\n", params[0].value.a,
			params[1].value.a);
	params[0].value.a /= params[1].value.a;
	IMSG("Result: %d", params[0].value.a);

	return TEE_SUCCESS;
}

/*
 * allows the TA to subtract values, but only subtractions with positive
 * results are allowed because TEE_Param is unsigned.
 */
static TEE_Result subtract_values(uint32_t paramTypes, TEE_Param params[4])
{
	uint32_t expParamTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
			TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);
	if (expParamTypes != paramTypes ||
		       	params[0].value.a < params[1].value.a) {
		return TEE_ERROR_BAD_PARAMETERS;
	}
	IMSG("calculationg %d - %d\n", params[0].value.a,
			params[1].value.a);
	params[0].value.a -= params[1].value.a;
	IMSG("Result: %d\n", params[0].value.a);

	return TEE_SUCCESS;
}

/*
 * Function is called whenever the client invokes a TA command.
 */
TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused  *sessionContext,
	       			uint32_t commandId,
				uint32_t paramTypes,
				TEE_Param params[4])
{

	/* Unused parameter */
	(void)&sessionContext;

	switch (commandId){
	case TA_CALCULATOR_ADD:
		return add_values(paramTypes, params);
	case TA_CALCULATOR_MULTIPLY:
		return multiply_values(paramTypes, params);
	case TA_CALCULATOR_DIVIDE:
		return divide_values(paramTypes, params);
	case TA_CALCULATOR_SUBTRACT:
		return subtract_values(paramTypes, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}	



