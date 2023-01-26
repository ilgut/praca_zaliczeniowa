#include <iostream>
#include <fstream>
#include <iomanip> //dla fixed oraz setprecision
#include <vector> 
#include <string>

#define DATABASE "database.txt"
#define BUYINGS "count_of_buyings.txt"

using namespace std;

namespace shop
{
    fstream stream;
    vector<string> computer_types = { "Laptop", "Komputer stacjonarny", "Nettop", "Monoblock" };    //vector z typami sprzetu
    struct Product  //struktura odpowiedzialna za dowolny artykul w sklepie.
    {
        int number;
        string type;
        string brand;
        string model;
        string cpu;
        int ram;
        double price;
    };

    //----------------------------------------------------------------------------------------------------------------

    int number_of_lines_in_file(string filename)
    {
        ifstream in(filename);
        string temp;
        int count = 0;
        while (getline(in, temp))
        {
            count++;
        }
        in.close();
        return count;
    }

    //----------------------------------------------------------------------------------------------------------------

    void split(string input, string output[7])
    {
        string temp;
        int pointer = 0;
        int len = input.length();

        for (int i = 0; i < len; i++)
        {
            if (input[i] == '|') // '|' -- znak, co oddziela informacji w wierszu z artykluem
            {
                output[pointer] = temp;
                temp.clear();
                ++pointer;
                continue;
            }
            temp += input[i];
        }
    }
    void print_all_items()
    {
        string str;
        string result[7];

        cout << "Lista wszystkich towarow : " << endl;

        stream.open(DATABASE, ios::in);

        while (getline(stream, str))
        {
            split(str, result);

            for (int i = 0; i < 7; i++)
            {
                cout << result[i] << " ";
            }
            cout << endl;
        }
        cout << endl;
        stream.close();
    }

    //-----------------------------------------------------------------------------------------------------------

    void add_item()
    {
        cout << "Zapraszamy w tryb dodawania pozycji! Postepuj zgodnie z ponizszymi instrukcjami : \n\n";
        Product product;
        while (true)
        {
            string type;
            cout << "Wprowadz typ urzadzenia : ";
            getline(cin, type);
            if (find(computer_types.begin(), computer_types.end(), type) != computer_types.end()) //walidacja tupy
            {
                product.type = type;
                break;
            }
            cout << "Nieprawidlowy format wejsciowy! Sprawdz napisanie i wielkie litery na poczatku slowa i sprobuj ponownie.\n";
            continue;
        }
        cout << "Wprowadz marke producenta : ";
        getline(cin, product.brand);
        cout << endl;
        cout << "Wprowadz nazwe modelu urzadzenia : ";
        getline(cin, product.model);
        cout << endl;
        cout << "Wprowadz nazwe procesora : ";
        getline(cin, product.cpu);
        cout << endl;
        while (true)
        {
            string temp;
            cout << "Wprowadz ilosc pamieci RAM (w gigabajtach) : ";
            getline(cin, temp);
            if (temp.find_first_not_of("0123456789") != string::npos) //walidacja
            {
                cout << "Nieprawidlowy format wejsciowy! Prosze wpisac ilosc RAM w liczbach.\n";
            }
            else
            {
                product.ram = stoi(temp);
                break;
            }
        }
        cout << endl;
        cout << "I ostatnia rzecz to cena. Wpisz cene, potem ta pozycja zostanie automatycznie zapisana w bazie sklepu (liczby ulamkowe wpisujemy z kropka)\nCena : ";
        while (true)
        {
            string temp;
            getline(cin, temp);
            if (temp.find_first_not_of("0123456789.") != string::npos)   //walidacja
            {
                cout << "Nieprawidlowy format wejsciowy! Prosze wpisac wartosc w liczbach. (liczby ulamkowe wpisujemy z kropka)\n";
            }
            else
            {
                product.price = stod(temp.c_str());
                break;
            }
        }

        cout << endl;
        product.number = number_of_lines_in_file(DATABASE) + 1;
        string new_item = product.type + '|' + product.brand + '|' + product.model + '|' + product.cpu + '|';
        new_item.append(to_string(product.ram) + "GB RAM|" + to_string(product.price).substr(0, to_string(product.price).find_first_of('.') + 3) + " ZL|");

        ifstream in(DATABASE);
        string temp;
        while (getline(in, temp))   //szukamy taki samy artykul
        {
            string test = temp.substr(temp.find_first_of('|') + 1);
            if (new_item == temp.substr(temp.find_first_of('|') + 1))
            {
                cout << "Znaleziono identyczny produkt w bazie danych! Nie mozesz dodac tej samej pozycji!\n" << endl;
                return;
            }
        }
        in.close();

        stream.open(DATABASE, ios::out | ios::app | ios::ate);      //wpisujemy do bazy danych
        stream << '\n';
        stream << product.number << ".|" << product.type << '|' << product.brand << '|'
            << product.model << '|' << product.cpu << '|'
            << product.ram << "GB RAM|" << fixed << setprecision(2) << product.price << " ZL|"; //fixed oraz setprecision dla prawidlowego wpisania liczby ulamkowej (2 cyfry po przecinku)

        stream.close();
    }

    //----------------------------------------------------------------------------------------------------------------

    void delete_item(int line_to_delete)
    {
        string line;
        int count = 0;
        stream.open(DATABASE, ios::in);
        ofstream out("database_temp.txt");  //tworzymy tymcasowa bazu danych

        while (getline(stream, line))
        {
            count += 1;
            if (count != line_to_delete)
            {
                if (count != 1)
                {
                    out << '\n';
                }
                if (count > line_to_delete)     //zmiana numeru pozycji w bazie danych
                {
                    string old_num = line.substr(0, line.find('.'));
                    int new_num = atoi(old_num.c_str()) - 1;
                    line = to_string(new_num) + line.substr(line.find('.'), line.length() - 1);
                }
                out << line;
            }
        }
        stream.close();
        out.close();

        remove(DATABASE);
        rename("database_temp.txt", DATABASE);  //robimy z tymaczsowej bazy danych glowna

        cout << endl << endl;
        print_all_items();
    }

    //----------------------------------------------------------------------------------------------------------------

    void change_item()
    {
        int position;
        print_all_items();
        while (true)
        {
            cout << "Wybierz numer pozycji, ktora chcesz edytowac : ";
            cin >> position;
            cin.get();
            if (cin.fail() || position <= 0 || position > number_of_lines_in_file(DATABASE))
            {
                cout << "Nieprawidlowy format wejsciowy!\n";
                continue;
            }
            else
            {
                break;
            }
        }

        string line;
        string line_to_be_changed[7];
        stream.open(DATABASE, ios::in);
        int count = 1;
        while (getline(stream, line))
        {
            if (count == position)
            {
                split(line, line_to_be_changed);
            }
            ++count;
        }
        stream.close();

        char answer;
        bool quit = true;
        string changed_info;
        while (true && quit)
        {
            cout << "\nWybierz ustawienie, ktore chcesz zmienic:\n\nTyp urzadzenia(t), marka producenta(m), nazwa modelu(n), procesor CPU(c), ilosc pamieci RAM(r), cena(p) : \nZakoncz edycje i zapisz zmiany -- q\n";
            cin >> answer;
            cin.get();
            cout << endl;

            switch (answer)
            {
            case 't':
                cout << "Wprowadz nowe dane : \n";
                getline(cin >> ws, changed_info);   // >> ws sluzy do poprawnego dzialania getline() po cin
                line_to_be_changed[1] = changed_info;
                break;
            case 'm':
                cout << "Wprowadz nowe dane : \n";
                getline(cin >> ws, changed_info);
                line_to_be_changed[2] = changed_info;
                break;
            case 'n':
                cout << "Wprowadz nowe dane : \n";
                getline(cin >> ws, changed_info);
                line_to_be_changed[3] = changed_info;
                break;
            case 'c':
                cout << "Wprowadz nowe dane : \n";
                getline(cin >> ws, changed_info);
                line_to_be_changed[4] = changed_info;
                break;
            case 'r':
                cout << "Wprowadz nowe dane : \n";
                getline(cin >> ws, changed_info);
                if (changed_info.find_first_not_of("0123456789") != string::npos) //walidacja
                {
                    cout << "Nieprawidlowy format wejsciowy! Prosze wpisac ilosc RAM w liczbach.\n\n";
                }
                else
                {
                    line_to_be_changed[5] = changed_info + "GB RAM";
                    break;
                }
                break;
            case 'p':
                cout << "Wprowadz nowe dane : \n";
                getline(cin >> ws, changed_info);
                if (changed_info.find_first_not_of("0123456789.") != string::npos) //walidacja
                {
                    cout << "Nieprawidlowy format wejsciowy! Prosze wpisac cene w liczbach.\n\n";
                }
                else
                {
                    line_to_be_changed[6] = changed_info + " ZL";
                    break;
                }
                break;
            case 'q':
                cout << "Zmiany zapisane!\n";
                quit = false;
                break;
            default:
                cout << "Nieprawidlowy format wejsciowy, sprobuj ponownie.\n\n" << endl;
                break;
            }
        }

        fstream stream;
        count = 0;
        stream.open(DATABASE, ios::in);
        ofstream out("database_temp.txt");

        while (getline(stream, line))
        {
            count += 1;
            if (count != 1)
            {
                out << '\n';
            }
            if (count == position)
            {
                out << line_to_be_changed[0] << '|' << line_to_be_changed[1] << '|' << line_to_be_changed[2] << '|'
                    << line_to_be_changed[3] << '|' << line_to_be_changed[4] << '|'
                    << line_to_be_changed[5] << '|' << line_to_be_changed[6] << '|';
                continue;
            }
            out << line;
        }
        stream.close();
        out.close();

        remove(DATABASE);
        rename("database_temp.txt", DATABASE);  //robimy z tymaczsowej bazy danych glowna
    }

    //----------------------------------------------------------------------------------------------------------------

    void search()
    {
        string input;
        cout << "\nWprowadz slowo kluczowe do wyszukania : \n";
        getline(cin, input);

        cout << "\nLista wynikow wyszukiwania : \n";
        stream.open(DATABASE, ios::in);
        string line;
        int count = 0;
        while (getline(stream, line))
        {
            if (line.find(input) != string::npos)   //jezeli znaleziony artykul, to wypisujemy go do konsoli
            {
                ++count;
                string parsed_line[7];
                split(line, parsed_line);
                parsed_line[0] = to_string(count) + ".";
                for (int i = 0; i < 7; i++)
                {
                    cout << parsed_line[i] << " ";
                }
                cout << endl;
            }
        }
        cout << endl;
        stream.close();
    }

    //----------------------------------------------------------------------------------------------------------------

    void report_price();
    void report_ram();
    void report_type();

    void reports()
    {
        cout << "\nWybierz typ filtrowania, dla ktorego chcesz otrzymac raport : \nc - wedlug ceny\nr - wedlug ilosci pamieci RAM\nt - wedlug typu urzadzenia\nq - Wyjscie\n";
        char desicion;
        while (true)
        {
            cin >> desicion;
            switch (desicion)
            {
            case 'c':
                report_price();
                break;
            case 'r':
                report_ram();
                break;
            case 't':
                report_type();
                break;
            case 'q':
                return;
            default:
                cout << "Nieprawidlowy format wejsciowy, sprobuj ponownie.\n";
                break;
            }
        }
    }

    void report_price()
    {
        string str;
        float price;
        cout << "\nWybrales filtrowanie wedlug ceny. Podaj cene ponizej ktorej lacznie chcesz zobaczyc produkt : ";
        while (true)
        {
            getline(cin >> ws, str);
            if (str.find_first_not_of("0123456789.") != string::npos)   //walidacja
            {
                cout << "\nNieprawidlowy format wejsciowy, sprobuj ponownie.\n";
                continue;
            }
            else
            {
                price = stof(str);
            }
            break;
        }

        cout << "\nWyniki : \n";
        stream.open(DATABASE, ios::in);
        string parsed_string[7];
        int count = 0;
        while (getline(stream, str))
        {
            split(str, parsed_string);
            if (price >= stof(parsed_string[6].substr(0, parsed_string[6].find(' '))))  // porownujemy ceny
            {
                ++count;
                parsed_string[0] = to_string(count) + '.';
                cout << parsed_string[0] << " " << parsed_string[1] << " " << parsed_string[2]  //wypisujemy do konsoli
                    << " " << parsed_string[3] << " " << parsed_string[4] << " "
                    << parsed_string[5] << " " << parsed_string[6] << endl;
            }
        }
        cout << endl;
        stream.close();
    }

    void report_ram()
    {
        string str;
        int ram;
        cout << "Wybrales filtrowanie wedlug ilosci pamieci RAM. Podaj dokladna ilosc pamieci RAM : ";
        while (true)
        {
            getline(cin >> ws, str);
            if (str.find_first_not_of("0123456789.") != string::npos)   //walidacja
            {
                cout << "\nNieprawidlowy format wejsciowy! Sprobuj ponownie.\n";
                continue;
            }
            else
            {
                ram = stoi(str);
            }
            break;
        }

        cout << "\nWynik : \n";
        stream.open(DATABASE, ios::in);
        string parsed_string[7];
        int count = 0;
        while (getline(stream, str))
        {
            split(str, parsed_string);
            if (ram == stoi(parsed_string[5].substr(0, parsed_string[5].find('G'))))    // porownujemy ram
            {
                ++count;
                parsed_string[0] = to_string(count) + '.';
                cout << parsed_string[0] << " " << parsed_string[1] << " " << parsed_string[2]  //wypisujemy do konsoli
                    << " " << parsed_string[3] << " " << parsed_string[4] << " "
                    << parsed_string[5] << " " << parsed_string[6] << endl;
            }
        }
        cout << endl;
        stream.close();
    }

    void report_type()
    {
        string type;
        while (true)
        {
            cout << "Wybrales filtrowanie wedlug typu sprzetu. Wprowadz konkretny typ, dla ktorego chcesz zobaczyc pelny raport : \n";
            getline(cin, type);
            if (find(computer_types.begin(), computer_types.end(), type) != computer_types.end())
            {
                break;
            }
            cout << "Nieprawidlowy format wejsciowy! Sprawdz napisanieu slowa i sprobuj ponownie.\n";
            continue;
        }

        cout << "\nWynik : \n";
        stream.open(DATABASE, ios::in);
        string line;
        string parsed_string[7];
        int count = 0;
        while (getline(stream, line))
        {
            split(line, parsed_string);
            if (parsed_string[1] == type)
            {
                ++count;
                parsed_string[0] = to_string(count) + '.';
                cout << parsed_string[0] << " " << parsed_string[1] << " " << parsed_string[2]
                    << " " << parsed_string[3] << " " << parsed_string[4] << " "
                    << parsed_string[5] << " " << parsed_string[6] << endl;
            }
        }
        cout << endl;
        stream.close();
    }

    //----------------------------------------------------------------------------------------------------------------

    void buy()
    {
        int choosed_item = 0;
        print_all_items();

        while (true)
        {
            cout << "\nCieszymy sie, ze jestes zainteresowany naszym produktem! Wybierz numer przedmiotu, ktory chcesz kupic : \n";
            cin >> choosed_item;
            cin.get();
            if (cin.fail())
            {
                cout << "Nieprawidlowy format wejsciowy!\n";
                continue;
            }
            else if (choosed_item <= 0 || choosed_item > number_of_lines_in_file(DATABASE))
            {
                cout << "Za mala lub za duza liczba!\n";
                continue;
            }
            else
            {
                break;
            }
        }

        string temp;
        string choosed_item_info[7];
        int count = 1;
        stream.open(DATABASE, ios::in | ios::out | ios::app);
        while (getline(stream, temp))
        {
            if (count == choosed_item)
            {
                split(temp, choosed_item_info);
                break;
            }
            ++count;
        }
        stream.close();

        cout << "Dobry wybor! Teraz wprowadz swoj numer telefonu, aby nasz menedzer skontaktowal sie z Toba i omowil metode platnosci i dostawe" << endl;
        string number;
        while (true)
        {
            cout << "Numer telefonu : " << endl;
            getline(cin, number);
            if (number.find_first_not_of("0123456789") != string::npos || number.length() != 9) //walidacja numeru
            {
                cout << "Nieprawidlowy format wejsciowy!\n";
                continue;
            }
            else
            {
                cout << "\nDziekujemy! Nasz menedzer wkrotce sie z Toba skontaktuje" << endl;
                break;
            }
        }

        temp = "";
        count = 1;
        ofstream out("count_of_buyings_temp.txt");
        stream.open(BUYINGS, ios::in);
        while (getline(stream, temp))       //zmieniamy informacje o ilosci zakupow
        {
            if (count != 1)
            {
                out << '\n';
            }
            if (temp.substr(0, temp.find_first_of('|')) == choosed_item_info[1]) // sprawdziamy typ zakupionego urzadzenia i znajdujemy odpowiednie
            {
                out << temp.substr(0, temp.find_first_of('|') + 1) << to_string(stoi(temp.substr(temp.find('|') + 1, temp.length())) + 1);
                ++count;
                continue;
            }
            out << temp;
            ++count;
        }
        stream.close();
        out.close();

        remove(BUYINGS);
        rename("count_of_buyings_temp.txt", BUYINGS);
    }

    //----------------------------------------------------------------------------------------------------------------

    void add_new_type()
    {
        cout << "Dostepne typy komputerow : " << endl;
        for (string type : computer_types)
        {
            cout << type << endl;
        }
        string new_type;
        cout << "\nPodaj nowy typ komputera, wtedy bedziesz mogl dodawac komputery tego typu do sklepu : " << endl;
        getline(cin, new_type);
        if (find(computer_types.begin(), computer_types.end(), new_type) == computer_types.end())
        {
            computer_types.push_back(new_type);
            cout << "Dodano nowy typ!\n" << endl;
            stream.open(BUYINGS, ios::out | ios::app | ios::ate);
            stream << '\n';
            stream << new_type << "|0";
            stream.close();
        }
        else
        {
            cout << "\nTen typ juz jest na liscie!\n" << endl;
            return;
        }
    }

    //----------------------------------------------------------------------------------------------------------------

    void analytics_popularity();
    void analytics_amount_of_each_type();
    int find_index_of_elem(vector<string>, string);

    void analytics()
    {
        char answer;
        cout << "Wybierz raport dla kazdego typu sprzetu: \n1 - Raport wedlug popularnosci\n2 - Wedlug liczby urzadzen kazdego typu" << endl;
        cin >> answer;
        cin.get();
        switch (answer)
        {
        case '1':
            analytics_popularity();
            break;
        case '2':
            analytics_amount_of_each_type();
            break;
        default:
            cout << "\nNieprawidlowy format wejsciowy!\n";
            return;
        }
    }

    void analytics_popularity()
    {
        cout << "\nWybrales raport wedlug popularnosci, czyli jakie typy sprzetu sa kupowane najczesciej. Wyniki: \n\n";
        string line;
        vector<string> all_buyings = vector<string>(number_of_lines_in_file(BUYINGS));
        int index = 0;
        stream.open(BUYINGS, ios::in);
        while (getline(stream, line))
        {
            all_buyings[index] = line;
            ++index;
        }
        stream.close();

        string temp;
        for (int i = 1; i < index; ++i)
        {
            //sortowanie
            if (stoi(all_buyings[i].substr(all_buyings[i].find('|') + 1, all_buyings[i].length())) > stoi(all_buyings[i - 1].substr(all_buyings[i - 1].find('|') + 1, all_buyings[i - 1].length())))
            {
                temp = all_buyings[i - 1];
                all_buyings[i - 1] = all_buyings[i];
                all_buyings[i] = temp;
                i = 0;
            }
        }
        //wypisujemy wynik
        for (int i = 0; i < index; i++)
        {
            cout << all_buyings[i].substr(0, all_buyings[i].find('|')) << " : " << all_buyings[i].substr(all_buyings[i].find('|') + 1, all_buyings[i].length()) << endl;
        }

        cout << "\nNajpopularniejsza pozycja -- " << all_buyings[0].substr(0, all_buyings[0].find_first_of('|')) << endl << endl;
    }

    int find_index_of_elem(vector<string> mas, string str)
    {
        for (int i = 0; i < 7; i++)
        {
            if (mas[i] == str)
                return i;
        }
        return -1;
    }

    void analytics_amount_of_each_type()
    {

        vector<int> amount = vector<int>(computer_types.size());
        for (int i = 0; i < computer_types.size(); i++)
        {
            amount[i] = 0;
        }

        string line;
        string splitted_line[7];
        stream.open(DATABASE, ios::in);
        while (getline(stream, line))
        {
            split(line, splitted_line);
            ++amount[find_index_of_elem(computer_types, splitted_line[1])];
        }
        stream.close();

        cout << "Typ sprzetu oraz ilosc towarow tego typu w naszym magazynie : \n" << endl;
        for (int i = 0; i < computer_types.size(); i++)
        {
            cout << computer_types[i] << " : " << amount[i] << endl;
        }

        cout << endl;
    }
}