



struct birthday_t{
  uint8_t month;
  uint8_t day;
};

struct birthday_t jesus = {12,24};



struct birthday_t events[] = {jesus};

bool check_birthdays(int day, int month){

  for(auto &ev : events){
    if(ev.day == day && ev.month-1 == month){
      return true;
    }
  }
  return false;
}

//////////////////////////////////
