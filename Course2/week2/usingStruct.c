//Write enumerated types that supports dates—such as december 12. Then add a function that produces a next day.  So nextday(date) of december 12 is december 13. Also write a function printdate(date) that prints a date legibly.The function can assume that February has 28 days and it most know how many days are in each month. Use a struct with two members; one is the month and the second  is the day of the month—an int (or short).


#include <stdio.h>


typedef enum month { jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec } month;
typedef struct date { enum month m; int d;} date;

static const char * const month_name[] = {
    "January",  "February", "March",
    "April",    "May",      "June",
    "July",     "August",   "September",
    "October",  "November", "December",
};

void print_month(struct date date) { //simple function for displaying month and day
    switch (date.m) {
      case jan:
      case feb:
      case mar:
      case apr:
      case may:
      case jun:
      case jul:
      case aug:
      case sep:
      case oct:
      case nov:
      case dec:
        printf("%s %d \n", month_name[date.m], date.d);
        break;
      default:
        printf("Out of range!\n");
        break;
    }
}

date next_day(struct date next) { //next month|day function which is the problem.
    int last_day;
    switch (next.m) {
      case jan:
      case mar:
      case may:
      case jul:
      case aug:
      case oct:
      case dec:
        last_day = 31;
        break;
      case apr:
      case jun:
      case sep:
      case nov:
        last_day = 30;
        break;
      case feb:
        last_day = 28;
        break;
      default:
        return next;
    }
    if (next.d++ == last_day) {
        next.d = 1;
        if (next.m++ == dec) {
            next.m = jan;
        }
    }
    return next;
}

int main() {
    struct date date_1 = { feb, 28};
    struct date date_2 = { mar, 14};
    struct date date_3 = { oct, 31};
    struct date date_4 = { dec, 31};
    struct date date_5 = { jan, 1};

    print_month(date_1);
    print_month(date_2);
    print_month(date_3);
    print_month(date_4);
    print_month(date_5);
    printf("\n");

    print_month(next_day(date_1));
    print_month(next_day(date_2));
    print_month(next_day(date_3));
    print_month(next_day(date_4));
    print_month(next_day(date_5));
    printf("\n\n");

    return 0;
}
