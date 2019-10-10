/*
 * /home/todd/ANT/gatt_one/bluez-5.18
 * make -j8
 * sudo make install
 * cd attrib
 * cp gatttool gatt_todd_tool
 * sudo cp gatt_todd_tool \usr\bin\gatt_todd_tool
 * cd \home\todd\Down*\hrm*\tra*
 * from ~\Downloads\hrm data\trainer_raw_data .truestandDATA
 * 
 * 
 * 
 * read https://www.jaredwolff.com/get-started-with-bluetooth-low-energy/
 * 
 *  need to obtain BlueZ
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2010  Nokia Corporation
 *  Copyright (C) 2010  Marcel Holtmann <marcel@holtmann.org>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <errno.h>
#include <glib.h>
#include <stdlib.h>
#include <unistd.h>

#include <GL/freeglut.h> //added
#include <GL/gl.h> //added

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "src/shared/util.h"
#include "lib/uuid.h"
#include "att.h"
#include "btio/btio.h"
#include "gattrib.h"
#include "gatt.h"
#include "gatttool.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <string.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> // added
#include "/home/todd/ANT/gatt_one/bluez-5.18/attrib/trig_functions.h" // added
#include "/home/todd/ANT/gatt_one/bluez-5.18/attrib/track.h" // added
#define WINDOW_WIDTH 800 //added
#define WINDOW_HEIGHT 800 //added
#define wait_time 60 // added
double wheel_circumference =(2096); // mm //added
double miles_per_kilometer= (0.621371); //added
double miles_goal = 27.0; // added
double global_miles; //added
double global_rival_miles; //added
double global_rival_speed; // added
double global_delta_rotations; //added
//below added
#define timespan 5

double global_previous_rotation;
char global_mileage_string[50];
char global_speed_string[50];
double global_individual_speeds[timespan];
int global_individual_speed_spot;
time_t global_start_time;


// from https://users.cs.jmu.edu/bernstdh/web/common/lectures/summary_opengl-text.php
void renderStrokeFontString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {

	char *c;
	glPushMatrix();
	glTranslatef(x, y,z);

	for (c=string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}

	glPopMatrix();
}


void drawString(float x, float y, float z, char *string) {
  glRasterPos3f(x, y, z);
  float deltaX = 0.10;
  glColor3f(1.0f,	0.0f,	0.0f);
  for (char* c = string; *c != '\0'; c++) {
//	  glutStrokeCharacter (GLUT_STROKE_ROMAN, *c);
//	  x = x + deltaX;
//	  glRasterPos3f(x, y, z);
     glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);  // Updates the position
  }
 //  glutStrokeCharacter (GLUT_STROKE_ROMAN, *c);
 
}
void draw_track()
{   int i; // for for loop
	//float angle; // do draw the arc
	float cx;
	float cy;
	float pi;
	float radius;
	float box;
	//float xp;
	//float yp;
	int trackspot;
	int rival_trackspot;
	glColor3f(0.0f, 0.0f, 1.0f);
	box = 0.005f;
	trackspot = floor((global_miles/track_length-floor(global_miles/track_length))*400.0);
	for (i=0;i<400;i++)
		//{if (i > trackspot)
			{	glRectf(	track_data[i][0]-box,	
							track_data[i][1]-box, 
							track_data[i][0]+box,	 
							track_data[i][1]+box);
			}
		//}	
	//trackspot = fmod(global_miles,track_length)*400;
	if (trackspot>400) trackspot=399;
	glRectf(	track_data[trackspot][0]-4*box,	
		    	track_data[trackspot][1]-4*box, 
				track_data[trackspot][0]+4*box,	 
				track_data[trackspot][1]+4*box);
	glColor3f(1.0f, 0.0f, 1.0f);
	global_rival_speed = -exp_todd(-global_rival_miles/8.61)*9.77+25.0;
	
	if ((global_rival_miles<miles_goal) &&(global_miles>0.0))
		  //global_rival_miles += (-exp_todd(-global_rival_miles/8.61*10.0)+25.0)/3600.0;
		  //                               =(-exp_todd(-global_rival_miles/8.61)*9.77+25.0
			//printf("-exp(-0.005) == %lf\n",-exp_todd(-0.005));
			
			//printf("rival speed == %lf rival_miles == %4.2lf !!!!!\n",
			 //                     (-exp_todd(-global_rival_miles/8.61)*9.77+25.0),global_rival_miles);
		   
			global_rival_miles += (-exp_todd(-global_rival_miles/8.61)*9.77+25.0)/3600.0; //typo fixed
			
			//(-global_rival_miles/8.61)*10.0+25.0)
			
			
				
	rival_trackspot = floor((global_rival_miles/track_length-floor(global_rival_miles/track_length))*400.0);	
	glRectf(	track_data[rival_trackspot][0]-4*box,	
		    	track_data[rival_trackspot][1]-4*box, 
				track_data[rival_trackspot][0]+4*box,	 
				track_data[rival_trackspot][1]+4*box);
						
	glColor3f(1.0f, 0.0f, 0.0f);
	for (i=0;i<trackspot;i++)
		{glRectf(	track_data[i][0]-box,	
					track_data[i][1]-box, 
					track_data[i][0]+box,	 
					track_data[i][1]+box);
		}
	glColor3f(0.0f, 0.0f, 1.0f);	
					
	//printf("trackspot is %i\n",trackspot);
		
	//global_miles
	// track_data[400][2]
	
	box = 0.005f;
	radius = 0.417544543351756;
	cx = 0.532455456648244;
	cy = -0.5625;
	pi = 3.14159265358979;
	//draw line piece by piece
	glColor3f(0.0f, 0.0f, 1.0f);
	/*for (i=0; i<=50; i++)
		{	glRectf(	0.0+0.532455456648244f/50.0*i-box,	
						-0.980044543351756f-box, 0.0+
						0.0+0.532455456648244f/50.0*i+box,	 
						-0.980044543351756f+box);
		}*/
	/*
	for (i=0; i<= 180; i =i+10)
		{	xp = radius*cosine_todd((-90+i)*pi/360.0);
			yp = radius*sine_todd((-90+i)*pi/360.0);
			glRectf(	0.532455456648244+xp-box,	
						-0.5625 +yp-box,
						0.532455456648244+xp+box,	 
						-0.5625 +yp+box);
						
		}*/
	/*for (i=0;i<=100; i++)
		{	glRectf(	0.532455456648244f-0.532455456648244f/50.0*i-box,	
						-0.144955456648244-box, 0.0+
						0.532455456648244f-0.532455456648244f/50.0*i+box,	 
						-0.144955456648244+box);
		}
	for (i=0;i<=50; i++)
		{	glRectf(	-0.532455456648244f+0.532455456648244f/50.0*i-box,	
						-0.980044543351756f-box, 0.0+
						-0.532455456648244f+0.532455456648244f/50.0*i+box,	 
						-0.980044543351756f+box);
		}*/
	
}
void display() {
	int i;
	float speed;
	char speed_string[50];
	
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       //clear_screen();
       drawString(-0.25f,	0.375f,	0.0f,global_mileage_string);
       draw_track();
       renderStrokeFontString(0.0f,	0.375f,	0.0f,GLUT_STROKE_ROMAN,global_mileage_string);
       //printf("%s", global_mileage_string);
       for (i=0;i<timespan;i++)
		{printf(" %4.3f ",global_individual_speeds[i]);}
	   printf("\n");
	   speed = 0.0f;   	
       for (i=0; i<timespan; i++) {speed = speed+ global_individual_speeds[i] ;}
       speed = speed / timespan * 3600.0/1000.0/1000.0*wheel_circumference*miles_per_kilometer;
       sprintf(speed_string,"Speed = %3.1f",speed);
       drawString(0.0f,	0.5f,	0.0f,speed_string);
       
       glColor3f(0.0f, 1.0f, 0.0f);
       glRectf(-0.75f,0.125f, 0.75f, -0.125f);
       glColor3f(1.0f, 0.0f, 0.0f);
       printf("global_miles == %f XXX rival_speed == %f XXX %f\n",global_miles,global_rival_speed,global_rival_miles);
       glRectf(-0.75f,0.075f, -0.75+1.5*global_miles/miles_goal, -0.075f);   
	
       glutSwapBuffers();
}


//above added

static char *opt_src = NULL;
static char *opt_dst = NULL;
static char *opt_dst_type = NULL;
static char *opt_value = NULL;
static char *opt_sec_level = NULL;
static bt_uuid_t *opt_uuid = NULL;
static int opt_start = 0x0001;
static int opt_end = 0xffff;
static int opt_handle = -1;
static int opt_mtu = 0;
static int opt_psm = 0;
static gboolean opt_primary = FALSE;
static gboolean opt_characteristics = FALSE;
static gboolean opt_char_read = FALSE;
static gboolean opt_listen = FALSE;
static gboolean opt_char_desc = FALSE;
static gboolean opt_char_write = FALSE;
static gboolean opt_char_write_req = FALSE;
static gboolean opt_interactive = FALSE;
static GMainLoop *event_loop;
static gboolean got_error = FALSE;
static GSourceFunc operation;

struct characteristic_data {
	GAttrib *attrib;
	uint16_t start;
	uint16_t end;
};

static void events_handler(const uint8_t *pdu, uint16_t len, gpointer user_data)
{
	GAttrib *attrib = user_data;
	uint8_t *opdu;
	uint16_t handle, i, olen = 0;
	size_t plen;
	int an_int; //added
	int rotations; //added
	int time_stamp; // added
	double average_speed; // added
	double miles_travelled; // added
			double seconds; //adeed
	
	
	handle = get_le16(&pdu[1]);
    
	switch (pdu[0]) {
	case ATT_OP_HANDLE_NOTIFY:
		//g_print("CCNotification handle = 0x%04x value: ", handle);
		//g_print("CCNotification handle = 0x%04i value: ", handle); // modified
		//below added 2019_September_20
		//an_int = handle; //added
		//printf("\n handle = ??????\n"); // added
		//printf("%4i %4i %4i %4i %4i %4i %4i %4i\n",pdu[0],pdu[1],pdu[2],pdu[3],pdu[4],pdu[5],pdu[6],pdu[7]);
		rotations = pdu[4] + pdu[5]*256 +pdu[6]*256*256+pdu[7]*256*256*256;
		global_individual_speeds[global_individual_speed_spot] = rotations-global_previous_rotation;
		global_previous_rotation = rotations;
		global_individual_speed_spot += 1;
		if (global_individual_speed_spot > timespan) {global_individual_speed_spot=0;}
		
		//global_delta_rotations = rotations - global_delta_rotations;
		//global_speed_array[global_speed_spot] = global_delta_rotations*wheel_circumference/1000.0/1000.0*miles_per_kilometer;
		//global_speed_spot +=1;
		//if (global_speed_spot>timespan) global_speed_spot = 0;
		time_stamp = pdu[8]+pdu[9]*256;
		miles_travelled = rotations/1000.0/1000.0*wheel_circumference*miles_per_kilometer;
		global_miles = miles_travelled;
		//for (i=0; i< timespan; i++)
		//	{average_speed += global_speed_array[i];}
		//average_speed = average_speed/timespan;
		//printf("Rotations == %i time_stamp = %i ///// miles = %f\n",rotations,time_stamp,miles_travelled);
		//printf("speeds := %4.3f %4.3f %4.3f %4.3f %4.3f\n",global_speed_array[0],global_speed_array[1],global_speed_array[2],global_speed_array[3],global_speed_array[4]);
		time_t timer;
		time(&timer);

		seconds = difftime(timer,global_start_time);
		
		sprintf(global_mileage_string,"miles =  %4.2f; rival = %4.2f delta %4.2f\n",global_miles,global_rival_miles,global_miles-global_rival_miles);
		//if (global_speed_spot>5) global_speed_spot=0;
		display();
		//drawString(0.0,	0.25,	0.0,mileage_string);
		//above added 2019_September_20
		break;
	case ATT_OP_HANDLE_IND:
		g_print("Indication   handle = 0x%04x value: ", handle);
		break;
	default:
		g_print("Invalid opcode\n");
		return;
	}

//	for (i = 3; i < len; i++) // commented out
//		g_print("%02x ", pdu[i]); // commented out
//
	g_print(" ");// \n delted

	if (pdu[0] == ATT_OP_HANDLE_NOTIFY)
		return;

	opdu = g_attrib_get_buffer(attrib, &plen);
	olen = enc_confirmation(opdu, plen);

	if (olen > 0)
		g_attrib_send(attrib, 0, opdu, olen, NULL, NULL, NULL);
}

static gboolean listen_start(gpointer user_data)
{
	GAttrib *attrib = user_data;

	g_attrib_register(attrib, ATT_OP_HANDLE_NOTIFY, GATTRIB_ALL_HANDLES,
						events_handler, attrib, NULL);
	g_attrib_register(attrib, ATT_OP_HANDLE_IND, GATTRIB_ALL_HANDLES,
						events_handler, attrib, NULL);

	return FALSE;
}

static void connect_cb(GIOChannel *io, GError *err, gpointer user_data)
{
	GAttrib *attrib;

	if (err) {
		g_printerr("%s\n", err->message);
		got_error = TRUE;
		g_main_loop_quit(event_loop);
	}

	attrib = g_attrib_new(io);

	if (opt_listen)
		g_idle_add(listen_start, attrib);

	operation(attrib);
}

static void primary_all_cb(uint8_t status, GSList *services, void *user_data)
{
	GSList *l;

	if (status) {
		g_printerr("Discover all primary services failed: %s\n",
							att_ecode2str(status));
		goto done;
	}

	for (l = services; l; l = l->next) {
		struct gatt_primary *prim = l->data;
		g_print("attr handle = 0x%04x, end grp handle = 0x%04x "
			"uuid: %s\n", prim->range.start, prim->range.end, prim->uuid);
	}

done:
	g_main_loop_quit(event_loop);
}

static void primary_by_uuid_cb(uint8_t status, GSList *ranges, void *user_data)
{
	GSList *l;

	if (status != 0) {
		g_printerr("Discover primary services by UUID failed: %s\n",
							att_ecode2str(status));
		goto done;
	}

	for (l = ranges; l; l = l->next) {
		struct att_range *range = l->data;
		g_print("Starting handle: %04x Ending handle: %04x\n",
						range->start, range->end);
	}

done:
	g_main_loop_quit(event_loop);
}

static gboolean primary(gpointer user_data)
{
	GAttrib *attrib = user_data;

	if (opt_uuid)
		gatt_discover_primary(attrib, opt_uuid, primary_by_uuid_cb,
									NULL);
	else
		gatt_discover_primary(attrib, NULL, primary_all_cb, NULL);

	return FALSE;
}

static void char_discovered_cb(uint8_t status, GSList *characteristics,
								void *user_data)
{
	GSList *l;

	if (status) {
		g_printerr("Discover all characteristics failed: %s\n",
							att_ecode2str(status));
		goto done;
	}

	for (l = characteristics; l; l = l->next) {
		struct gatt_char *chars = l->data;

		g_print("handle = 0x%04x, char properties = 0x%02x, char value "
			"handle = 0x%04x, uuid = %s\n", chars->handle,
			chars->properties, chars->value_handle, chars->uuid);
	}

done:
	g_main_loop_quit(event_loop);
}

static gboolean characteristics(gpointer user_data)
{
	GAttrib *attrib = user_data;

	gatt_discover_char(attrib, opt_start, opt_end, opt_uuid,
						char_discovered_cb, NULL);

	return FALSE;
}

static void char_read_cb(guint8 status, const guint8 *pdu, guint16 plen,
							gpointer user_data)
{
	uint8_t value[plen];
	ssize_t vlen;
	int i;

	if (status != 0) {
		g_printerr("Characteristic value/descriptor read failed: %s\n",
							att_ecode2str(status));
		goto done;
	}

	vlen = dec_read_resp(pdu, plen, value, sizeof(value));
	if (vlen < 0) {
		g_printerr("Protocol error\n");
		goto done;
	}
	g_print("Characteristic value/descriptor: ");
	for (i = 0; i < vlen; i++)
		g_print("%02x ", value[i]);
	g_print("\n");

done:
	if (!opt_listen)
		g_main_loop_quit(event_loop);
}

static void char_read_by_uuid_cb(guint8 status, const guint8 *pdu,
					guint16 plen, gpointer user_data)
{
	struct att_data_list *list;
	int i;

	if (status != 0) {
		g_printerr("Read characteristics by UUID failed: %s\n",
							att_ecode2str(status));
		goto done;
	}

	list = dec_read_by_type_resp(pdu, plen);
	if (list == NULL)
		goto done;

	for (i = 0; i < list->num; i++) {
		uint8_t *value = list->data[i];
		int j;

		g_print("handle: 0x%04x \t value: ", get_le16(value));
		value += 2;
		for (j = 0; j < list->len - 2; j++, value++)
			g_print("%02x ", *value);
		g_print("\n");
	}

	att_data_list_free(list);

done:
	g_main_loop_quit(event_loop);
}

static gboolean characteristics_read(gpointer user_data)
{
	GAttrib *attrib = user_data;

	if (opt_uuid != NULL) {

		gatt_read_char_by_uuid(attrib, opt_start, opt_end, opt_uuid,
						char_read_by_uuid_cb, NULL);

		return FALSE;
	}

	if (opt_handle <= 0) {
		g_printerr("A valid handle is required\n");
		g_main_loop_quit(event_loop);
		return FALSE;
	}

	gatt_read_char(attrib, opt_handle, char_read_cb, attrib);

	return FALSE;
}

static void mainloop_quit(gpointer user_data)
{
	uint8_t *value = user_data;

	g_free(value);
	g_main_loop_quit(event_loop);
}

static gboolean characteristics_write(gpointer user_data)
{
	GAttrib *attrib = user_data;
	uint8_t *value;
	size_t len;

	if (opt_handle <= 0) {
		g_printerr("A valid handle is required\n");
		goto error;
	}

	if (opt_value == NULL || opt_value[0] == '\0') {
		g_printerr("A value is required\n");
		goto error;
	}

	len = gatt_attr_data_from_string(opt_value, &value);
	if (len == 0) {
		g_printerr("Invalid value\n");
		goto error;
	}

	gatt_write_cmd(attrib, opt_handle, value, len, mainloop_quit, value);

	return FALSE;

error:
	g_main_loop_quit(event_loop);
	return FALSE;
}

static void char_write_req_cb(guint8 status, const guint8 *pdu, guint16 plen,
							gpointer user_data)
{
	if (status != 0) {
		g_printerr("Characteristic Write Request failed: "
						"%s\n", att_ecode2str(status));
		goto done;
	}

	if (!dec_write_resp(pdu, plen) && !dec_exec_write_resp(pdu, plen)) {
		g_printerr("Protocol error\n");
		goto done;
	}

	g_print("Characteristic value was written successfully\n");

done:
	if (!opt_listen)
		g_main_loop_quit(event_loop);
}

static gboolean characteristics_write_req(gpointer user_data)
{
	GAttrib *attrib = user_data;
	uint8_t *value;
	size_t len;

	if (opt_handle <= 0) {
		g_printerr("A valid handle is required\n");
		goto error;
	}

	if (opt_value == NULL || opt_value[0] == '\0') {
		g_printerr("A value is required\n");
		goto error;
	}

	len = gatt_attr_data_from_string(opt_value, &value);
	if (len == 0) {
		g_printerr("Invalid value\n");
		goto error;
	}

	gatt_write_char(attrib, opt_handle, value, len, char_write_req_cb,
									NULL);

	return FALSE;

error:
	g_main_loop_quit(event_loop);
	return FALSE;
}

static void char_desc_cb(guint8 status, const guint8 *pdu, guint16 plen,
							gpointer user_data)
{
	struct att_data_list *list;
	guint8 format;
	int i;

	if (status != 0) {
		g_printerr("Discover all characteristic descriptors failed: "
						"%s\n", att_ecode2str(status));
		goto done;
	}

	list = dec_find_info_resp(pdu, plen, &format);
	if (list == NULL)
		goto done;

	for (i = 0; i < list->num; i++) {
		char uuidstr[MAX_LEN_UUID_STR];
		uint16_t handle;
		uint8_t *value;
		bt_uuid_t uuid;

		value = list->data[i];
		handle = get_le16(value);

		if (format == ATT_FIND_INFO_RESP_FMT_16BIT)
			bt_uuid16_create(&uuid, get_le16(&value[2]));
		else {
			uint128_t u128;

			/* Converts from LE to BE byte order */
			bswap_128(&value[2], &u128);
			bt_uuid128_create(&uuid, u128);
		}

		bt_uuid_to_string(&uuid, uuidstr, MAX_LEN_UUID_STR);
		g_print("handle = 0x%04x, uuid = %s\n", handle, uuidstr);
	}

	att_data_list_free(list);

done:
	if (!opt_listen)
		g_main_loop_quit(event_loop);
}

static gboolean characteristics_desc(gpointer user_data)
{
	GAttrib *attrib = user_data;

	gatt_discover_char_desc(attrib, opt_start, opt_end, char_desc_cb, NULL);

	return FALSE;
}

static gboolean parse_uuid(const char *key, const char *value,
				gpointer user_data, GError **error)
{
	if (!value)
		return FALSE;

	opt_uuid = g_try_malloc(sizeof(bt_uuid_t));
	if (opt_uuid == NULL)
		return FALSE;

	if (bt_string_to_uuid(opt_uuid, value) < 0)
		return FALSE;

	return TRUE;
}

static GOptionEntry primary_char_options[] = {
	{ "start", 's' , 0, G_OPTION_ARG_INT, &opt_start,
		"Starting handle(optional)", "0x0001" },
	{ "end", 'e' , 0, G_OPTION_ARG_INT, &opt_end,
		"Ending handle(optional)", "0xffff" },
	{ "uuid", 'u', G_OPTION_FLAG_OPTIONAL_ARG, G_OPTION_ARG_CALLBACK,
		parse_uuid, "UUID16 or UUID128(optional)", "0x1801"},
	{ NULL },
};

static GOptionEntry char_rw_options[] = {
	{ "handle", 'a' , 0, G_OPTION_ARG_INT, &opt_handle,
		"Read/Write characteristic by handle(required)", "0x0001" },
	{ "value", 'n' , 0, G_OPTION_ARG_STRING, &opt_value,
		"Write characteristic value (required for write operation)",
		"0x0001" },
	{NULL},
};

static GOptionEntry gatt_options[] = {
	{ "primary", 0, 0, G_OPTION_ARG_NONE, &opt_primary,
		"Primary Service Discovery", NULL },
	{ "characteristics", 0, 0, G_OPTION_ARG_NONE, &opt_characteristics,
		"Characteristics Discovery", NULL },
	{ "char-read", 0, 0, G_OPTION_ARG_NONE, &opt_char_read,
		"Characteristics Value/Descriptor Read", NULL },
	{ "char-write", 0, 0, G_OPTION_ARG_NONE, &opt_char_write,
		"Characteristics Value Write Without Response (Write Command)",
		NULL },
	{ "char-write-req", 0, 0, G_OPTION_ARG_NONE, &opt_char_write_req,
		"Characteristics Value Write (Write Request)", NULL },
	{ "char-desc", 0, 0, G_OPTION_ARG_NONE, &opt_char_desc,
		"Characteristics Descriptor Discovery", NULL },
	{ "listen", 0, 0, G_OPTION_ARG_NONE, &opt_listen,
		"Listen for notifications and indications", NULL },
	{ "interactive", 'I', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_NONE,
		&opt_interactive, "Use interactive mode", NULL },
	{ NULL },
};

static GOptionEntry options[] = {
	{ "adapter", 'i', 0, G_OPTION_ARG_STRING, &opt_src,
		"Specify local adapter interface", "hciX" },
	{ "device", 'b', 0, G_OPTION_ARG_STRING, &opt_dst,
		"Specify remote Bluetooth address", "MAC" },
	{ "addr-type", 't', 0, G_OPTION_ARG_STRING, &opt_dst_type,
		"Set LE address type. Default: public", "[public | random]"},
	{ "mtu", 'm', 0, G_OPTION_ARG_INT, &opt_mtu,
		"Specify the MTU size", "MTU" },
	{ "psm", 'p', 0, G_OPTION_ARG_INT, &opt_psm,
		"Specify the PSM for GATT/ATT over BR/EDR", "PSM" },
	{ "sec-level", 'l', 0, G_OPTION_ARG_STRING, &opt_sec_level,
		"Set security level. Default: low", "[low | medium | high]"},
	{ NULL },
};
//below added
void clearscreen()
	{glClearColor(1.0, 1.0, 1.0, 1.0); //white background
	glClear(GL_COLOR_BUFFER_BIT);
	
	}
static void init01(void) 
{
   glClearColor (1.0, 1.0, 1.0, 0.0);  // clear to color
   glShadeModel (GL_FLAT); // The default value is GL_SMOOTH
}
void clear_screen()
{		glClearColor(1.0, 1.0, 1.0, 1.0); //white background
		glClear(GL_COLOR_BUFFER_BIT);
		glEnd();
		glBegin(GL_QUADS);
		glColor3f(0.0,	1.0,	0.0);
		glVertex2f(0.99,	0.99);
		glVertex2f(-0.99,	0.99);
		glVertex2f(-0.99,	-0.99);
		glVertex2f(0.99,	-0.99);
		glColor3f(1.0,	0.0,	0.0);
}	
void setup() {
	   //global_speed_spot = 0;
	   //global_speed_array[0] = 0.0;
	   //global_speed_array[1] = 0.0;
	   //global_speed_array[2] = 0.0;
	   //global_speed_array[3] = 0.0;
	   //global_speed_array[4] = 0.0;
       glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//above added
int main(int argc, char *argv[])
{
	//below added 2019_September_20
	int GLargc;    //created for gluInit
	char **GLargv; //created for gluInit
	int i;
	
	
	time(&global_start_time);
    //global_delta_rotations = 0.0;
    time_t timer;
    double seconds;
    double old_seconds;
    GLargc=0;
	GLargv=0;
	global_individual_speed_spot = 0;
	for (i=0; i<timespan; i ++)
		{global_individual_speeds[i] = 0.0f;}
	glutInit(&GLargc,GLargv);		
	//glutInitDisplayMode(GLUT_SINGLE);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // 800 x 800 
    //glutInitWindowPosition(100, 200);
    glutCreateWindow("OpenGL - Cre@ting a speedometer");	
    seconds = difftime(timer,global_start_time);
    old_seconds =seconds;
    while (seconds<51) // add delay here 51seconds
		{   time(&timer);
			seconds = difftime(timer,global_start_time);
			//printf("%f\n",old_seconds);
			//if (seconds - old_seconds >0.1)
				//{printf("diff time == %f\n",seconds);
				// old_seconds=seconds;	}
				 }
	printf("GO\n");
	printf("  GO\n");
	printf("    GO\n");
	
	 
	global_previous_rotation = 0.0;
	global_miles = 0.0;
	global_rival_miles= 0.0;
    setup();
    glutDisplayFunc(display);
    //glutMainLoop();
    glutMainLoopEvent();
    //glBegin(GL_QUADS);
    //init01;
    //clearscreen();
    
    //glClearColor(0.0, 1.0, 0.0, 1.0); //green background
    //glClear(GL_COLOR_BUFFER_BIT);
    //glColor3f(0.0, 0.0, 0.0);
    //clear_screen();
    
	//above added 2019_September_20
	GOptionContext *context;
	GOptionGroup *gatt_group, *params_group, *char_rw_group;
	GError *gerr = NULL;
	GIOChannel *chan;

	opt_dst_type = g_strdup("public");
	opt_sec_level = g_strdup("low");

	context = g_option_context_new(NULL);
	g_option_context_add_main_entries(context, options, NULL);

	/* GATT commands */
	gatt_group = g_option_group_new("gatt", "GATT commands",
					"Show all GATT commands", NULL, NULL);
	g_option_context_add_group(context, gatt_group);
	g_option_group_add_entries(gatt_group, gatt_options);

	/* Primary Services and Characteristics arguments */
	params_group = g_option_group_new("params",
			"Primary Services/Characteristics arguments",
			"Show all Primary Services/Characteristics arguments",
			NULL, NULL);
	g_option_context_add_group(context, params_group);
	g_option_group_add_entries(params_group, primary_char_options);

	/* Characteristics value/descriptor read/write arguments */
	char_rw_group = g_option_group_new("char-read-write",
		"Characteristics Value/Descriptor Read/Write arguments",
		"Show all Characteristics Value/Descriptor Read/Write "
		"arguments",
		NULL, NULL);
	g_option_context_add_group(context, char_rw_group);
	g_option_group_add_entries(char_rw_group, char_rw_options);

	if (!g_option_context_parse(context, &argc, &argv, &gerr)) {
		g_printerr("%s\n", gerr->message);
		g_clear_error(&gerr);
	}

	if (opt_interactive) {
		interactive(opt_src, opt_dst, opt_dst_type, opt_psm);
		goto done;
	}

	if (opt_primary)
		operation = primary;
	else if (opt_characteristics)
		operation = characteristics;
	else if (opt_char_read)
		operation = characteristics_read;
	else if (opt_char_write)
		operation = characteristics_write;
	else if (opt_char_write_req)
		operation = characteristics_write_req;
	else if (opt_char_desc)
		operation = characteristics_desc;
	else {
		char *help = g_option_context_get_help(context, TRUE, NULL);
		g_print("%s\n", help);
		g_free(help);
		got_error = TRUE;
		goto done;
	}

	if (opt_dst == NULL) {
		g_print("Remote Bluetooth address required\n");
		got_error = TRUE;
		goto done;
	}

	chan = gatt_connect(opt_src, opt_dst, opt_dst_type, opt_sec_level,
					opt_psm, opt_mtu, connect_cb, &gerr);
	if (chan == NULL) {
		g_printerr("%s\n", gerr->message);
		g_clear_error(&gerr);
		got_error = TRUE;
		goto done;
	}

	event_loop = g_main_loop_new(NULL, FALSE);

	g_main_loop_run(event_loop);

	g_main_loop_unref(event_loop);

done:
	g_option_context_free(context);
	g_free(opt_src);
	g_free(opt_dst);
	g_free(opt_uuid);
	g_free(opt_sec_level);
//below added 2019_September_20
    glEnd();
	glFlush();
	glutMainLoop();
//above added 2019_September_20
	if (got_error)
		exit(EXIT_FAILURE);
	else
		exit(EXIT_SUCCESS);
}
