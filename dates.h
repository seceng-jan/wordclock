



struct birthday_t{
  uint8_t month;
  uint8_t day;
};

struct birthday_t l = {1, 1};
struct birthday_t a = {2, 1};
struct birthday_t b = {3, 1};


struct birthday_t events[] = {a,b,l};

bool check_birthdays(int day, int month){

  for(auto &ev : events){
    if(ev.day == day && ev.month-1 == month){
      return true;
    }
  }
  return false;
}

//////////////////////////////////
