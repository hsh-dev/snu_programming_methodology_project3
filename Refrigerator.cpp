#include "Refrigerator.h"
#include "Food.h"
#include "NormalController.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <limits>

using namespace std;

/**
 * A constructor of the class
 * Controller should be initialized in this constructor
 */
Refrigerator::Refrigerator() : size(30, 20)
{
    controller = new NormalController(size, foodList);
}

/**
 * A constructor of the class
 * @param size_ the size of the refrigerator (must be positive value)
 * Controller should be initialized in this constructor
 */
Refrigerator::Refrigerator(intPair size_)
{
    if (size_.first <= 0 or size_.second <= 0)
    {
        std::cout << "The size of refrigerator must be positive value";
        exit(1);
    }
    size = size_;

    controller = new NormalController(size, foodList);
}

/**
 * base function of the refrigerator
 * User should type a number between 1 and 4 to call a function of the regrigerator
 * Then, this function utilizes the other functions of the refrigerator as the user's input
 * Below is the list of options that menuSelect should support
 * 1) display
 * 2) insert food
 *    2-1) insert a list of foods from a text file (.txt)
 *    2-2) insert a food directly
 * 3) pop food
 * 4) end program
 * Unless the user ends the program ( 4) end program ),
 * this function does not terminate and keeps requiring the user to type a number
 * @return 0 when the user ends the program by typing else 1
 */
int Refrigerator::menuSelect()
{
    int i;
    while (1)
    {
        cout << "************************************************************" << endl;
        cout << "************************   Menu   **************************" << endl;
        cout << "************************************************************" << endl;

        cout << "1. display" << endl;
        cout << "2. insert food" << endl;
        cout << "3. pop food" << endl;
        cout << "4. end program" << endl;
        cout << "Select the function you want : ";
        cin >> i;
        if (i < 1 || i > 4)
        {
            cout << "Type an integer from 1 to 4!" << endl;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        else
        {
            break;
        }
    }
    if (i == 4)
    {
        return 0;
    }
    else
    {
        switch (i)
        {
            case 1:
                display();
                break;
            case 2:
            {
                int j;
                while (1)
                {
                    cout << "1. Insert a list of foods from a text file" << endl;
                    cout << "2. Insert a food directly" << endl;
                    cout << "Enter how to insert your food : ";
                    cin >> j;
                    if (!(j == 1 or j == 2))
                    {
                        cout << "Type 1 or 2 only" << endl;
                        if (cin.fail())
                        {
                            cin.clear();
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                if (j == 1)
                    insertFoodFromFile();
                else
                    insertFoodDirectly();
                break;
            }
            case 3:
            {
                popFood();
                break;
            }
        }
        return 1;
    }
}

/**
 * This function print out the storage state and the food lists to the program output window.
 * This function should call the display function of controller first,
 * print outs the foods with the different exp dates
 * e.g. [bacon,3] exp date : 3
 *      [bacon,2] exp date : 1
 *      [egg,3] exp date : 2
 */
void Refrigerator::display()
{
    // grid display
    controller->display();

    // text list
    foodListType::iterator iter;
    for (iter = foodList.begin(); iter != foodList.end(); ++iter)
    {
        map<int, int> foodCounter;
        for (int i = 0; i < (iter->second).size(); i++)
        {
            int exp = (iter->second)[i]->getExp();
            foodCounter[exp]++;
        }
        map<int, int>::iterator iter_fc;
        for (iter_fc = foodCounter.begin(); iter_fc != foodCounter.end(); ++iter_fc)
        {
            cout << '[' << (iter->first) << ',' << (iter_fc->second) << ']'
                 << " exp date : " << (iter_fc->first) << endl;
        }
    }
}

/**
 * Get food informations from user and stack them into refrigerator by calling 
 * the stackFood function in Controller class.
 */
void Refrigerator::insertFoodDirectly()
{
    string foodName;
    intPair foodSize;
    int expDate;
    cout << "Enter food info" << endl;
    cout << "Food name : ";
    cin >> foodName;
    cout << "Food size :";
    cin >> foodSize.first >> foodSize.second;
    cout << "Days left to expiration date : ";
    cin >> expDate;

    /**
     * ===============================================
     * ========== TODO: Implement this part ==========
     * ===============================================
     */
    controller->stackFood(foodName, foodSize, expDate);
}

/**
 * Receive a name of text file containing food information lists from user and read it. 
 * Then, get the food informations line by line and stack foods into refrigerator 
 * by calling the `stackFood` function in Controller class.
 */
void Refrigerator::insertFoodFromFile()
{
    string foodFileName;
    cout << "Enter your file name : ";
    cin >> foodFileName;
    ifstream ff(foodFileName);
    if (!ff)
    {
        cout << "There is no food file!" << endl;
        return;
    }
    else
    {
        /**
         * ===============================================
         * ========== TODO: Implement this part ==========
         * ===============================================
         */
        string foodName;
        intPair foodSize;
        int number;
        int expDate;

        while (ff >> foodName) {
            ff >> foodSize.first >> foodSize.second >> number >> expDate;
            for(auto i = 0; i < number; i++){
                controller->stackFood(foodName, foodSize, expDate);
            }
        }
    }
    ff.close();
}

/**
 * Receives a 'foodName' from user and removes it from the refrigerator.
 * You should call the `popFood` function in Controller class.
 */
void Refrigerator::popFood()
{
    /**
     * ===============================================
     * ======== TODO: Implement this function ========
     * ===============================================
     */
    string foodname;
    cout << "Which food do you want to pop? : ";
    // input foodname to Controller popFood()
    cin >> foodname;
    controller -> popFood(foodname);
}


