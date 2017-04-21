/*
Группа людей называется современниками если был такой момент, когда они могли собраться вместе. 
Для этого в этот момент каждому из них должно было уже исполниться 18 лет, но ещё не исполниться 80 лет. 
Дан список Жизни Великих Людей. Необходимо получить максимальное количество современников. 
В день 18летия человек уже может принимать участие в собраниях, а в день 80летия и в день смерти уже не может. 
Замечание. Человек мог не дожить до 18-летия, либо умереть в день 18-летия. В этих случаях принимать участие в 
собраниях он не мог.
*/

#include <iostream>
#include <vector>

//Структура день-месяц-год
struct Date {
    int day, mounth, year;
    
    Date(int _day = 0, int _mounth = 0, int _year = 0) {
        day = _day;
        mounth = _mounth;
        year = _year;
    }
    
    bool operator<(Date &second) {
        return year < second.year ||
        (year == second.year &&
         (mounth < second.mounth ||
          (mounth == second.mounth && day < second.day)));
    }
    
    bool operator>(Date &second) {
        return year > second.year ||
        (year == second.year &&
         (mounth > second.mounth ||
          (mounth == second.mounth && day > second.day)));
    }
    
    bool operator==(Date &second) {
        return year == second.year && mounth == second.mounth && day == second.day;
    }
};

// Мог/не мог принимать участие в собраниях с какой-то даты 
struct active {
    Date date;
    bool is_active;
    
    active(struct Date _date = 0, bool _is_active = true) {
        date = _date;
        is_active = _is_active;
    }
    
    bool operator<(active &second) {
        return date < second.date;
    }
};

class human {
private:
    Date born, death;   //Дата рождения и смерти
    active start_active, end_active;    //С какой даты мог, и с какой не мог принимать участие в собраниях 
    bool is_active;     //Мог за свою жизнь принимать участие в собраниях
    
public:
    human() {}
    human(Date _born, Date _death): born(_born), death(_death), is_active(false) {
        Date eighteen = _born, eighty = _born;
        eighteen.year += 18;    //Дата 18-летия
        eighty.year += 80;     //Дата 80-летия
        
        if (death > eighteen) {
            is_active = true;
            start_active = active(eighteen);
            
            if (death > eighty) {
                end_active = active(eighty, false);
            } else {
                end_active = active(death, false);
            }
        }
    }
    
    bool get_is_avtive() const { return is_active; }
    
    active get_start_active() const { return start_active; }
    
    active get_end_active() const { return end_active; }
};

void heapify(std::vector<active> &array, int n, int i) {
    active temp = array[i];
    while (i < n / 2) {
        //пока не вышли за грань массива   
        int child = 2 * i + 1; // находим первого сына
        if (child < n) {
            if (child + 1 < n && array[child] < array[child + 1])
                child++;
            if (array[i] < array[child]) {
                // если наибольший из сыновей больше чем предок, то меняем их местами
                std::swap(array[i], array[child]);
                i = child;  // теперь предком является следующий элемент
            } else
                break; //куча с головой в i построена.
        } else
            break;
    }
    array[i] = temp;
}

void heap_sort(std::vector<active> &array) {
    for (int i = (int)array.size() / 2 - 1; i >= 0; i--) {
        heapify(array, (int)array.size(), i);
    }
    for (int i = (int)array.size() - 1; i > 0; i--) {
        std::swap(array[0], array[i]);
        heapify(array, i, 0);
    }
}

int get_count(std::vector<active> &array) {
    int max = 0, count = 0;
    
    for (int i = 0; i < array.size(); i++) {
        array[i].is_active ? count++ : count--; //добавился или наоборот участник в собраниях
        if (count > max)
            max = count;
    }
    return max;
}

int main() {
    int n = 0;
    std::cin >> n;
    
    std::vector<human> people;
    std::vector<active> times;
    
    int b_day = 0, b_mounth = 0, b_year = 0, d_day = 0, d_mounth = 0, d_year = 0;
    
    for (int i = 0; i < n; i++) {
        std::cin >> b_day >> b_mounth >> b_year >> d_day >> d_mounth >> d_year;
        
        human temp(Date(b_day, b_mounth, b_year), Date(d_day, d_mounth, d_year));
        people.push_back(temp);
        
        if (people[i].get_is_avtive()) {
            times.push_back(people[i].get_start_active());
            times.push_back(people[i].get_end_active());
        }
    }
    
    heap_sort(times);
    
    std::cout << get_count(times);
    
    return 0;
}
