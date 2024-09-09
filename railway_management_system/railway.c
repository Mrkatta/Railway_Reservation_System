#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRAINS 5
#define MAX_RESERVATIONS 100

typedef struct {
    char name[50];
    int train_num;
    int num_of_seats;
    int booking_id;
} Reservation;

typedef struct {
    int train_num;
    char train_name[50];
    char route[50];
    float fare;
    int available_seats;
} Train;

Train trains[MAX_TRAINS] = {
    {1001, "Southern Breeze Express", "Chennai-Bangalore", 1500.0, 30},
    {1002, "Cauvery Express",  "Chennai-Mysore", 1800.0, 30},
    {1003, "Coromandel Express", "Chennai-Kolkata", 2500.0, 30},
    {1004, "Nethravathi Express", "Trivandrum-Mangalore", 2000.0, 30},
    {1005, "Konkan Kanya Express", "Goa-Mumbai", 2200.0, 30}
};

Reservation reservations[MAX_RESERVATIONS];
int reservation_count = 0;

char password[10] = "pass"; // Default password

void reservation(void);
void viewdetails(void);
void cancel(void);
float charge(int, int);
void printticket(char name[], int, int, float, int);
void login(void);
void reset_password(void);
void show_main_menu(void);
int generate_booking_id(void);
int find_train(int train_num);
void update_seats(int train_num, int seats);

int main() {
    login();
    int choice;
    do {
        system("cls");
        printf("\n====== Main Menu ======\n");
        show_main_menu();
        printf("--> ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: reservation(); break;
            case 2: viewdetails(); getchar(); getchar(); break;
            case 3: cancel(); getchar(); break;
            case 4: reset_password(); getchar(); break;
        }
    } while (choice != 5);
    return 0;
}

void viewdetails(void) {
    system("cls");
    printf("\n====== Available Trains ======\n");
    printf("\nTrain No\tTrain Name\t\t\tFrom-To\t\t\tCharges\tAvailable Seats\n");
    printf("------------------------------------------------------------------------------\n");
    for (int i = 0; i < MAX_TRAINS; i++) {
        printf("%d\t\t%s\t%s\t\t%.2f\t%d\n", trains[i].train_num, trains[i].train_name, trains[i].route, trains[i].fare, trains[i].available_seats);
    }
    getchar(); // Wait for user input before returning
}

void reservation(void) {
    if (reservation_count >= MAX_RESERVATIONS) {
        printf("Sorry, no more reservations can be made.\n");
        return;
    }

    Reservation new_reservation;
    float charges;
    int train_index;

    system("cls");
    printf("\n====== Reservation ======\n");
    printf("Enter Name: ");
    getchar();
    fgets(new_reservation.name, sizeof(new_reservation.name), stdin);
    new_reservation.name[strcspn(new_reservation.name, "\n")] = 0;

    printf("Enter Number of Seats: ");
    scanf("%d", &new_reservation.num_of_seats);

    viewdetails();
    printf("Enter Train Number: ");
    scanf("%d", &new_reservation.train_num);

    train_index = find_train(new_reservation.train_num);
    if (train_index == -1) {
        printf("Invalid train number.\n");
        return;
    }

    if (trains[train_index].available_seats < new_reservation.num_of_seats) {
        printf("Not enough seats available.\n");
        return;
    }

    charges = charge(new_reservation.train_num, new_reservation.num_of_seats);
    new_reservation.booking_id = generate_booking_id();

    printticket(new_reservation.name, new_reservation.num_of_seats, new_reservation.train_num, charges, new_reservation.booking_id);

    printf("\nConfirm Ticket (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    if (confirm == 'y') {
        reservations[reservation_count++] = new_reservation;
        update_seats(new_reservation.train_num, -new_reservation.num_of_seats);
        printf("Reservation Successful!\n");
    } else {
        printf("Reservation Cancelled.\n");
    }

    getchar();
}

float charge(int train_num, int num_of_seats) {
    int train_index = find_train(train_num);
    if (train_index != -1) {
        return trains[train_index].fare * num_of_seats;
    }
    return 0;
}

void printticket(char name[], int num_of_seats, int train_num, float charges, int booking_id) {
    system("cls");
    printf("\n====== Ticket ======\n");
    printf("Name: %s\n", name);
    printf("Seats: %d\n", num_of_seats);
    printf("Train Number: %d\n", train_num);
    printf("Booking ID: %d\n", booking_id);
    printf("Charges: %.2f\n", charges);
}

void cancel(void) {
    int booking_id, found = -1;

    system("cls");
    printf("\n====== Cancel Reservation ======\n");
    printf("Enter Booking ID to Cancel: ");
    scanf("%d", &booking_id);

    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].booking_id == booking_id) {
            found = i;
            break;
        }
    }

    if (found != -1) {
        printticket(reservations[found].name, reservations[found].num_of_seats, reservations[found].train_num, charge(reservations[found].train_num, reservations[found].num_of_seats), reservations[found].booking_id);
        update_seats(reservations[found].train_num, reservations[found].num_of_seats);
        printf("\nReservation Cancelled.\n");

        for (int i = found; i < reservation_count - 1; i++) {
            reservations[i] = reservations[i + 1];
        }
        reservation_count--;
    } else {
        printf("Booking ID not found.\n");
    }

    getchar();
}

void reset_password(void) {
    char old_password[10], new_password[10];
    system("cls");
    printf("\n====== Reset Password ======\n");

    printf("Enter Old Password: ");
    scanf("%s", old_password);

    if (strcmp(old_password, password) == 0) {
        printf("Enter New Password: ");
        scanf("%s", new_password);
        strcpy(password, new_password);
        printf("Password Reset Successful!\n");
    } else {
        printf("Incorrect Old Password!\n");
    }
}

void login(void) {
    char user[10], pass[10], uname[] = "user";
    for (int i = 0; i < 3; i++) {
        system("cls");
        printf("\n====== Login ======\n");
        printf("Username: ");
        scanf("%s", user);
        printf("Password: ");
        scanf("%s", pass);

        if (strcmp(user, uname) == 0 && strcmp(pass, password) == 0) {
            printf("Login Successful!\n");
            getchar();
            return;
        }
        printf("Invalid Credentials. Try again.\n");
    }
    printf("Too many failed attempts. Exiting...\n");
    exit(0);
}

void show_main_menu(void) {
    printf("1 >> Make a Reservation\n");
    printf("2 >> View Available Trains\n");
    printf("3 >> Cancel a Reservation\n");
    printf("4 >> Reset Password\n");
    printf("5 >> Exit\n");
}

int generate_booking_id(void) {
    srand(time(0));  // Seed for random number generation
    return rand() % 10000 + 1;  // Generate random number between 1 and 10000
}

int find_train(int train_num) {
    for (int i = 0; i < MAX_TRAINS; i++) {
        if (trains[i].train_num == train_num) {
            return i;
        }
    }
    return -1;  // Train not found
}

void update_seats(int train_num, int seats) {
    int train_index = find_train(train_num);
    if (train_index != -1) {
        trains[train_index].available_seats += seats;
    }
}
