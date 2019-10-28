#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "board.h"
#include "xtimer.h"
#include "periph/pwm.h"
#include "servo.h"
#include "math.h"

#include "shell.h"

#define DEV         PWM_DEV(0)
#define CHANNEL     2
#define SERVO_MIN	(500U)
#define SERVO_MAX 	(5500U)


#define STILLSTAND	(1500U) //Bei dieser Zeit steht der Motor still. Sollte dies nicht der Fall sein muss er kalibriert werden.
#define MAXGESCHW	(1300U) //Bei dieser Zeit fährt der Motor auf voller Geschwindigkeit vorwärts
#define MINGESCHW	(1700U) //Bei dieser Zeit fährt der Motor auf voller Geschwindigkeit rückwärts

//#define MINGESCHW	(2680U)



static servo_t servo;

double d = 5; //Durchmesser der Spindel in cm


//static xtimer_t timer;

/*static int _alternativesAnsteuern(int argc, char**argv)
{
    if (argc != 2) {
        printf("usage: %s <value> \n", argv[0]);
        return 1;
    }
		
}*/

static int _kalibrieren(int argc, char**argv)
{
	if (argc !=1) {
		printf("keine Parameter erforderlich\n");
		printf("%s",argv[0]); //nur um Fehlermeldung zu verhindern
	

	}
	servo_set(&servo, STILLSTAND);
	printf("Stellen Sie mithlfe des Potis den Motor auf Stillstand\n");
	return 0;
}


static int _set(int argc, char**argv)
{
    if (argc != 2) {
        printf("usage: %s <value> \n", argv[0]);
        return 1;
    }

    unsigned dutycycle= atoi(argv[1]);


    servo_set(&servo, dutycycle);
    printf("auf %u gesetzt \n",dutycycle);
    return 0;
}

static int _geschw(int argc, char**argv)
{
	//int schritte= STILLSTAND-MAXGESCHW;
	if (argc != 2) {
		printf("Geben Sie %s und einen Wert zwischen -200 und +200 ein", argv[0]);	
		return 1;
	}
	
	unsigned v = atoi(argv[1]);		

	//if (v > 20 || v < -10) {
	//	printf("Geben Sie einen Wert zwischen -10 und 20 ein");
	//	return 1;
	//}
	servo_set(&servo, STILLSTAND-v);
	printf("set %u\n",STILLSTAND-v);
	return 0;
}




static const shell_command_t shell_commands[] = { //Liste der Shell-Befehle
    { "set", "set servo value", _set },
    { "v" , "Geschwindigkeit (+/-) eingeben", _geschw},
    {"kalibrieren" , "Stellt den Motor auf einen Standartwert, der mithilfe des Potis als Nullpunkt eingestellt werden muss.", _kalibrieren},
    { NULL, NULL, NULL }
};






int main(void)
{
    int res;


    puts("\nMotorsteuerung des RIOT-Projekts.");
    puts("Verbinden Sie den PWM-Eingang des Motors mit dem sechsten Pin von unten auf der rechten Seite");

    res = servo_init(&servo, DEV, CHANNEL, SERVO_MIN, SERVO_MAX);
    if (res < 0) {
        puts("Errors while initializing servo");
        return -1;
    }
    puts("Servo initialized.");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
