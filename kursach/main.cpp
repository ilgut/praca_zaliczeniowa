#include <iostream>
#include <fstream>
#include "source.h"

using namespace std;
using namespace shop;

int main()
{
    cout << "\t\t\tWITAMY W SKLEPIE ELEKTRONICZNYM \"LAPTOP-31\"***" << endl;
    cout << "Posiadamy rozne modele laptopow, komputerow stacjonarnych, nettopy oraz monoblocki." << endl;
    string decision;
    string choose;
    while (true)
    {
        cout << "Wybierz dzialanie z ponizszych : " << endl;
        cout << "Wyswietl wszystkie dostepne pozycje (1)\ndodaj pozycje do listy (2)\nusun pozycje (3)\n";
        cout << "edytuj pozycje (4)\nszukaj po slowie kluczowym (5)\nwyswietlaj raporty wedlug filtrow (6)\nkup produkt (7)\ndodaj nowy typ komputerow (8)";
        cout << "\ngenerowanie raportow analitycznych (9)" << endl;
        cout << "Skoncz prace (q)" << endl;
        getline(cin, decision);
        switch (decision[0])
        {
        case '1':
            print_all_items();
            break;
        case '2':
            add_item();
            break;
        case '3':
        {
            int line_to_delete = 0;
            print_all_items();

            while (true)
            {
                cout << "Wpisz numer urzadzenia na liscie, ktore chcesz usunac z bazy : ";
                cin >> line_to_delete;
                cin.get();
                if (cin.fail())
                {
                    cout << "Nieprawidlowy format wejsciowy!\n";
                    continue;
                }
                else if (line_to_delete <= 0 || line_to_delete > number_of_lines_in_file(DATABASE))
                {
                    cout << "Za mala lub za duza liczba!\n";
                    continue;
                }
                else
                {
                    break;
                }
            }

            delete_item(line_to_delete);
            break;
        }
        case '4':
            change_item();
            break;
        case '5':
            search();
            break;
        case '6':
            cout << "Wedlug jakiego filtra chcesz zobaczyc raport?\nWedlug ceny(1), ilosci pamieci RAM(2), typu urzadzenia(3): ";
            getline(cin, choose);
            switch (choose[0])
            {
            case '1':
                report_price();
                break;
            case '2':
                report_ram();
                break;
            case '3':
                report_type();
                break;
            default:
                cout << "Nieprawidlowy format wejsiowy!\n\n";
                break;
            }
            break;
        case '7':
            buy();
            break;
        case '8':
            add_new_type();
            break;
        case '9':
            analytics();
            break;
        case 'q':
            cout << "\nDziekujemy za korzystanie z naszego programu! Milego dnia!\n";
            return 0;
        default:
            cout << "Nieprawidlowy format wejsciowy!\n\n";
            break;
        }
    }
}