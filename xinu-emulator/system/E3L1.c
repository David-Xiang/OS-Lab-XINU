#include <xinu.h>
#include <custom.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXN 10

/*
	Exercise3 of Lab1
	Design a producer-consumer program using semaphore function provided
	by Xinu
	I design an ocassion in restaurant: there're one cook and several
	diners. The cook can serve DISHES(<=10) kinds of dishes and he cook 
	dishes in order (0.5s per dish). There're DINERS diners and they 
	always throw a dice to decide which dish he is going to have. It will
	cost a diner 2 seconds to finish the dish.

	NOTICE: it's not allowed to init multiple E3L1 process, otherwise the
	following globle variables will be altered.
 */

sid32 sem[MAXN];
int DISHES;
int DINERS;

void cook(int total){
	int count = 0;
	int choice = 0;
	while(count < total){
		choice = count % DISHES;
		signal(sem[choice]);
		count++;
		kprintf("Cook: serve dish %d, %d dishes served\n", choice, count);
		sleep(0.1);
	}
}
void diner(int id){
	int dice = 0;
	int count = 0;
	while(TRUE){
		dice = rand() % DISHES;
		wait(sem[dice]);
		count++;
		kprintf("Diner %d: get dish %d, %d dishes had\n", id, dice, count);
		sleep(0.2);
	}
}
void E3L1(int total, int ndishes, int ndiners){
	DISHES = ndishes;
	DINERS = ndiners;
	int i;
	sid32 sid;
	srand(getpid());

	// create semaphores
	for (i = 0; i < DISHES; i++){
		// create new semaphore and the initial value is 0
		sid = semcreate(0);
		if (sid == SYSERR){
			kprintf("Error encountered when creating semaphores!\n");
			return;
		}

		sem[i] = sid;
	}

	// create a cook
	resume(create(cook, 4096, 60, "cook", 1, total));

	// create diners
	char name[MAXN] = "";
	for (i = 0; i < DINERS; i++){
		sprintf(name, "%s %d", "diner", i);
		resume(create(diner, 4096, 65, name, 1, i));
	}

	return;
}
