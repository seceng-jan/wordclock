



struct birthday_t{
  uint8_t month;
  uint8_t day;
};

struct birthday_t a = {12, 25};


struct birthday_t events[] = {a};

bool check_birthdays(int day, int month){

  for(auto &ev : events){
    if(ev.day == day && ev.month-1 == month){
      return true;
    }
  }
  return false;
}

//////////////////////////////////
