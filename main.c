#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

const char *day_names[] = {
    "Su", // nday",
    "Mo", // nday",
    "Tu", // esday",
    "We", // dnesday",
    "Th", // ursday",
    "Fr", // iday",
    "Sa", // turday",
};

const char *month_names[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December",
};

int now_year, now_month, now_day;

bool is_similar(const char* text, const char* month) {
    if (strlen(month) < strlen(text))
        return false;
    for (const char *str=text;*str;str++)
        if (tolower(*str) != tolower(month[str-text]))
            return false;
    return true;
}

int resolve_month(const char* month){
    {
        int value = atoi(month);
        if (1 <= value && value <= 12) 
            return value - 1;
    }

    size_t sim_count = 0;
    size_t sim = 0;
    for (size_t i = 0; i < 12; i++)
        if (is_similar(month, month_names[i])) {
            sim_count++;
            sim = i;
        }
    
    if (sim_count == 1) return sim;
    return -1;
}

int get_num_days(int month, bool leap){
    switch(month){
        case 0: return 31;
        case 1: return leap ? 29 : 28;
        case 2: return 31;
        case 3: return 30;
        case 4: return 31;
        case 5: return 30;
        case 6: return 31;
        case 7: return 31;
        case 8: return 30;
        case 9: return 31;
        case 10: return 30;
        case 11: return 31;
        default:return 31;
    }
}

bool is_leap(int year){
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int get_day_start(int year, int month){
    int res = 1; // first day in calendar starts with monday (gregorian calendar)

    for(int i = 1;i<year;i++){ // There is no 0 AD, calendar starts with 1 AD
        res += is_leap(i) ? 2 : 1;
        if(res>=7)res %= 7;
    }

    bool leap = is_leap(year);
    for(int i=0;i<month;i++){
        res += get_num_days(i,leap)%7;
        if(res>=7)res %= 7;
    }
    if(res>=7)res %= 7;
    return res;
}

void print_centered_text(const char* text, size_t width){
    int text_length = strlen(text);

    // Calculate padding spaces
    int padding_total = width - text_length;
    int padding_left = padding_total / 2;
    int padding_right = padding_total - padding_left; // Accounts for odd padding

    // Print with calculated padding
    printf("%*s%s%*s\n", padding_left, "", text, padding_right, "");
}

void print_month(int year, int month){
    int day_start = get_day_start(year, month);
    int num_days = get_num_days(month, is_leap(year));

    print_centered_text(month_names[month], 20);
    printf("\033[36m====================\033[0m\n");

    // Print week names
    for(int i=0;i<7;i++){
        if (i == 0)
            printf("\033[91m%s\033[0m", day_names[i]);
        else 
            printf("\033[97m%s\033[0m", day_names[i]);
        if(i!=6)printf(" ");
        else printf("\n");
    }

    for(int i=0;i<day_start+num_days;i++){
        if(i<day_start) printf("  ");
        else { 
            int day = i + 1 - day_start;
            if (now_year == year && now_month == month && now_day == day) {
                printf("\033[30m\033[107m%2d\033[0m", day);
            } else
                printf("%2d", day);
        }

        if(i%7==6)printf("\n");
        else if (i < day_start + num_days - 1) printf(" ");
    }

    printf("\n");
}

void print_year(int year) {
    char buf[100+9];
    snprintf(buf, 100, "Year: %d AD", year);
    print_centered_text(buf, 20);
}

int main(){
    char year[100] = {0};
    char month[100] = {0};

    { // calculate now
        now_year = now_month = now_day = -1;
        time_t now = time(NULL);
        struct tm buf;
        if (now != (time_t)(-1)) 
            if (localtime_r(&now, &buf) != NULL) {
                now_year = buf.tm_year + 1900;
                now_month = buf.tm_mon;
                now_day = buf.tm_mday;
            }
    }

    printf("Enter year: ");
    if (scanf("%s", year) == EOF) memset(year, 0, 100);
    printf("Enter month: ");
    if (scanf("%s", month) == EOF) memset(year, 0, 100);

    int year_idx;
    if (strcmp(year, ".") == 0) {
        year_idx = now_year;
    } else {
        year_idx = atoi(year);
        if (year_idx <= 0) {
            fprintf(stderr, "Invalid year\n");
            return -1;
        }
    }

    if (strcmp(month, "*") == 0) {
        print_year(year_idx);
        for (size_t i = 0; i < 12; i++) {
            print_month(year_idx, i);
            if (i<11) printf("\n");
        }
        return 0;
    }

    int month_idx;
    if (strcmp(month, ".") == 0) {
        month_idx = now_month;
    } else {
        month_idx = resolve_month(month);
    }
    if(month_idx == -1) { 
        fprintf(stderr, "Invalid month\n"); 
        return -1; 
    } 
    print_year(year_idx);
    print_month(year_idx, month_idx);
    return 0;
}
