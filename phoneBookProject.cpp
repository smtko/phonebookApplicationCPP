#include<iostream>
#include<mysql.h>
#include<mysqld_error.h>
#include<windows.h>
#include<string.h>
using namespace std;

const char* HOST = "localhost";
const char* USER = "root";
const char* PW = "password";
const char* DB = "projectdb";

void printMenu()
{
	cout<<"1. Add contact: "<<endl;
	cout<<"2. Search contact: "<<endl;
	cout<<"3. Delete a contact: "<<endl;
	cout<<"4. Show all contacts: "<<endl;
	cout<<"5. Exit"<<endl;
}

int main()
{
	MYSQL *conn;
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0))
	{
		cout<<"Error: "<<mysql_error(conn)<<endl;
	}
	bool exit = false;
	printMenu();
	while(!exit)
	{
		int choice;
		cout<<"\nEnter choice: ";
		cin>>choice;
		if(choice < 1 || choice > 5)
		{
			cout<<"Invalid input! ";
			continue;
		}
		if(choice == 5)
		{
			cout<<"Bye!";
			exit = true;
			continue;
		}
		if(choice == 1)
		{
			string number, name;
			cout<<"Enter Mobile Number: ";
			cin>>number;
			cin.ignore();
			cout<<"Enter contact name: ";
			getline(cin, name);
			
			string qr = "INSERT INTO clist (Mob, Name) VALUES ('"+number+"', '"+name+"')";
			if(mysql_query(conn, qr.c_str()))
			{
				cout<<"Error: "<<mysql_error(conn)<<endl;
			}
			else
			{
				cout<<"Contact added..."<<endl;
			}
		}
		else if(choice == 2)
		{
			string s;
			cout<<"Search by number or name: ";
			cin.ignore();
			getline(cin, s);
			
			string qr = "SELECT Mob, Name FROM clist WHERE Mob LIKE '%"+s+"%' OR Name LIKE '%"+s+"%' ";
			if(mysql_query(conn, qr.c_str()))
			{
				cout<<"Error: "<<mysql_error(conn)<<endl;
			}
			else
			{
				MYSQL_RES *res = mysql_store_result(conn);
				MYSQL_ROW row;
				if(res == NULL)
				{
					cout<<"Error: "<<mysql_error(conn)<<endl;
				}
				else
				{
					bool found = false;
					while((row = mysql_fetch_row(res)))
                    {
                        found = true;
                        cout<<"["<<row[0]<<" | "<<row[1]<<"]"<<endl;
                    }
					if(!found)
					{
						cout<<"No contacts found! "<<endl;
					}
					mysql_free_result(res);
				}
			}
		}
		else if(choice == 3)
		{
			int deleteType;
            cout << "Delete by: 1. Mobile Number  2. Name" << endl;
            cout << "Enter your choice: ";
            cin >> deleteType;

            string deleteQuery;
            if (deleteType == 1) // Delete by Mobile Number
            {
                string number;
                cout << "Enter Mobile Number: ";
                cin >> number;

                deleteQuery = "DELETE FROM clist WHERE Mob = '" + number + "'";
            }
            else if (deleteType == 2) // Delete by Name
            {
                string name;
                cout << "Enter Name: ";
                cin.ignore(); // Clear buffer
                getline(cin, name);

                deleteQuery = "DELETE FROM clist WHERE Name = '" + name + "'";
            }
            else
            {
                cout << "Invalid delete option!" << endl;
                continue;
            }

            if(mysql_query(conn, deleteQuery.c_str()))
            {
                cout << "Error: " << mysql_error(conn) << endl;
            }
            else
            {
                if(mysql_affected_rows(conn) > 0)
                {
                    cout << "Contact deleted successfully!" << endl;
                }
                else
                {
                    cout << "No contact found to delete!" << endl;
                }
            }
		}
		else if(choice == 4)
		{
			string qr = "SELECT * FROM clist";
			if(mysql_query(conn, qr.c_str()))
			{
				cout<<"Error: "<<mysql_error(conn)<<endl;
			}
			else
			{
				MYSQL_RES *res = mysql_store_result(conn);
				MYSQL_ROW row;
				int cnt = 1;
				cout<<endl;
				while(row = mysql_fetch_row(res))
				{
					cout<<cnt<<") "<<row[0]<<" "<<row[1]<<endl;
					cnt++;
				}
				mysql_free_result(res);
			}
		}
	}
	return 0;
}
