#include <iostream>
#include <models/user_model.hpp>
#include <models/todo_model.hpp>

using namespace std;

int main()
{
	User u(1, "Mycin", "mycin.mit@gmail.com", "123");
	Todo t(1, u, "Brush", "Brush your teeth", false);

	cout << u.getUser() << endl;

	cout << t.getTodo() << endl;
    return 0;
}
