/******************************************************************************/
/* PiHAT - A software based home automation transmitter for the Raspberry PI  */
/*                                                                            */
/* Based on the work of the PiFM-guys, PiHAT enables you to control wireless  */
/* remote power switches with a Raspberry Pi with no extra hardware!          */
/* For proof of concept, simply connect a wire to GPIO 4,                     */
/* but for long term use a 433.92 MHz band pass filter _must_ be used!        */
/*                                                                            */
/* Use of this application is solely at your own risk!                        */
/*                                                                            */
/* Original PiFm-project: http://www.icrobotics.co.uk/wiki/index.php/         */
/*                        Turning_the_Raspberry_Pi_Into_an_FM_Transmitter     */
/*                                                                            */
/* PiHAT project: http://skagmo.com -> Projects -> PiHAT                      */
/*                                                                            */
/*                          Jon Petter Skagmo, 2012                           */
/******************************************************************************/

#include <inttypes.h>
#include <stdlib.h>
#include <argp.h>
#include <string.h>

#define BRAND_NEXA 			0x01
#define BRAND_EVERFLOURISH 	0x02
#define BRAND_WAVEMAN	 	0x03
#define BRAND_KANGHTAI	 	0x04
#define BRAND_STATUS		0x05

/***** Argp configuration start *****/

const char *argp_program_version = "PiHAT 0.2";
const char *argp_program_bug_address = "<web@skagmo.com>";
static char doc[] = "PiHAT -- Raspberry Pi Home Automation Transmitter.";
static char args_doc[] = "";

struct arguments{
	uint8_t brand;
	uint32_t id;
	uint8_t state;
	uint8_t channel;
	uint8_t group;
	uint8_t dim_level;
	uint8_t repeats;
};

static struct argp_option options[] = {
	{"brand",   'b', "BRAND", 0, "Brand of the receiver - defaults to Nexa (the only option as of now)"},
	{"id",   'i', "ID_FIELD", 0, "The unique ID field (integer)"},
	{"state",  's', "STATE", 0, "Power state: '0', '1'"},
	{"channel",  'c', "CHANNEL", 0, "Channel: '0' - '15'"},
	{"group",  'g', "GROUP_ON", 0, "Set group-flag: '0', '1'"},
	{"dim",  'd', "DIM_LEVEL", 0, "Dim-level for Nexa: '0' - '15'"},
	{"repeats",  'r', "REPEAT_NR", 0, "Number of times to resend packet: '0' - '255'"},
	{0}
};

/* Option parser */
static error_t parse_opt (int key, char *arg, struct argp_state *state){
	struct arguments *arguments = state->input;
	char* end; 	// Used to indicate if ASCII to int was successful

	switch (key){
		/* Brand */
		case 'b':
			if (!strcmp(arg, "n")) arguments->brand = BRAND_NEXA;
			else argp_usage(state);
			break;
    	/* ID */
		case 'i':
			arguments->id = strtol(arg, &end, 10);	// Convert ASCII-number to integer
			if (*end) argp_usage(state);
			break;
    	/* State */
		case 's':
			if (!strcmp(arg, "1")) arguments->state = 1;
			else if (!strcmp(arg, "0")) arguments->state = 0;
			else argp_usage(state);
			break;
		/* Channel */
		case 'c':
			arguments->channel = strtol(arg, &end, 10);
			if ( (*end) || (arguments->channel > 15) ) argp_usage(state);
			break;
		/* Group enable */
		case 'g':
			if (!strcmp(arg, "1")) arguments->group = 1;
			else if (!strcmp(arg, "0")) arguments->group = 0;
			else argp_usage(state);
			break;
		/* Dim-level */
		case 'd':
			arguments->dim_level = strtol(arg, &end, 10);
			if ( (*end) || (arguments->dim_level > 15) ) argp_usage(state);
			break;
		/* Number of repetitions */
		case 'r':
			arguments->repeats = strtol(arg, &end, 10);
			if (*end) argp_usage(state);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

/***** ARGP configuration stop *****/

#include "radio.h"
#include "nexa.h"
#include "status.h"

int main (int argc, char **argv){
	struct arguments arguments;

	/* Set argument defaults */
	arguments.brand = BRAND_NEXA;
	arguments.id = 0;
	arguments.state = 0;
	arguments.channel = 0;
	arguments.group = 0;
	arguments.dim_level = 0x10;	// Invalid entry - to determine if dim_level has been set at a later time
	arguments.repeats = 3;

	/* Parse arguments */
	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	/* Data variables for RF-packets */
	uint64_t data;			// General purpose data variable

	/* Setup RF-configuration */
	setup_io();
	setup_fm();
	ACCESS(CM_GP0DIV) = (0x5a << 24) + 0x374F; // Tune to 144.64 MHz to get the third harmonic at 433.92 MHz

	switch(arguments.brand){
		case BRAND_NEXA:
			/* Fill the data-variable and call the TX-function */
			data = 0;
			data |= ((uint64_t)arguments.id) << 10;
			data |= ((uint64_t)arguments.group) << 9;
			data |= ((uint64_t)arguments.state) << 8;
			data |= arguments.channel << 4;
			if (arguments.dim_level != 0x10) data |= arguments.dim_level;
			nexaTxPacket(&data, (arguments.dim_level != 0x10), arguments.repeats);
			break;
                case BRAND_STATUS:
                        data = 0;
                        data |= ((uint64_t)1) << 21;
                        data |= ((uint64_t)arguments.id) << 5;
                        data |= ((uint64_t)arguments.state) << 1;
                        statusTxPacket(&data, arguments.repeats);
                        break;
	}

	return 0;
}

