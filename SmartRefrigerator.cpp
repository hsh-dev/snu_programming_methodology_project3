#include "SmartRefrigerator.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Food.h"
#include "Recipe.h"

/**
 * A constructor of the class
 * SmartController should be initialized in this constructor
 */
SmartRefrigerator::SmartRefrigerator()
{
    controller = new SmartController(size, foodList);
};

/**
 * add recipes from a text file (.txt)
 * This function should add a new recipe object with the given info from the
 * file to the private variable recipe (vector)
 */
void SmartRefrigerator::addRecipeFromFile()
{
    string recipeFile;
    cout << "Enter your file name : ";
    cin >> recipeFile;
    ifstream recipe_list(recipeFile);

    if (!recipe_list)
    {
        cout << "There is no recipe file!" << endl;
        return;
    }
    /**
     * ===============================================
     * ======== TODO: Implement this function ========
     * ===============================================
     */
    string recipeName;
    strIntPair ingredient;
    vector<strIntPair> ingredients;
    double recipeScore;

    Recipe newRecipe;

    while(recipe_list >> recipeName) {
        recipe_list >> ingredient.first;
        while(ingredient.first != "/") {
            recipe_list >> ingredient.second;
            ingredients.push_back(ingredient);
            recipe_list >> ingredient.first;
        }
        recipe_list >> recipeScore;

        recipes.push_back(Recipe(recipeName, ingredients, recipeScore));
        ingredients.clear();
    }

    recipe_list.close();
}

/**
 * show the current recipes of the smart refrigetrator
 * e.g. recipe name : [ingredient1, # of ingredient1]...[] / score 7
 *
 */
void SmartRefrigerator::showRecipe()
{
    recipeVec ::iterator iter;
    if (recipes.empty())
        cout << "There is no recipe yet." << endl;
    else
    {
        for (iter = recipes.begin(); iter != recipes.end(); ++iter)
        {
            cout << iter->getName() << " : ";
            for (int i = 0; i < iter->getIngredients().size(); i++)
            {
                cout << '[' << iter->getIngredients()[i].first << ',' << iter->getIngredients()[i].second << ']';
            }
            cout << " / score " << iter->getScore() << endl;
        }
    }
}

/**
 * recommend the best possible combination (with repetition) of three recipes
 * Below are some rules about the recommendation
 * 1) the number of each ingredient required by a combination should be less
 * than the number of each ingredient in the refrigerator in order to be
 * considered as a possible combination 2) the sum of recipe scores of a
 * combination should be normalized by the highest score sum of the possible
 * combination 3) the exp sum of the foods in the refrigerator considering of a
 * combination should be normalized by the longest exp (the greatest number)
 * from a possible combination
 */
void SmartRefrigerator::recommendMealCourses() {
  /**
   * ===============================================
   * ======== TODO: Implement this function ========
   * ===============================================
   */
    typedef vector<string> Meal3;
    typedef pair<Meal3, pair<double, double>> Course; 
    vector<Course> courses;
    vector<Course> best;
    

    for (auto i = recipes.begin(); i != recipes.end(); ++i) {
        for (auto j = i + 1; j != recipes.end(); ++j) {
            for (auto k = j + 1; k != recipes.end(); ++k) {

                map<string, map<int, int>> usable_meal;
                Meal3 newMeal;

                for (foodListType::iterator iter = foodList.begin(); iter != foodList.end(); ++iter)
                {
                    map<int, int> foodCounter;
                    for (int l = 0; l < (iter->second).size(); l++)
                    {
                        int exp = (iter->second)[l]->getExp();
                        foodCounter[exp]++;
                    }

                    usable_meal.insert(make_pair(iter->first, foodCounter));
                }
                
                vector<strIntPair> ingredients = i->getIngredients();
                vector<strIntPair> temp = j->getIngredients();
                for (auto l : temp) {
                    bool check = true;
                    for (auto m : ingredients) {
                        if (m.first == l.first) {
                            check = false;
                            m.second += l.second;
                        }
                    }
                    if (check)
                        ingredients.push_back(l);
                }
                temp = k->getIngredients();
                for (auto l : temp) {
                    bool check = true;
                    for (auto m : ingredients) {
                        if (m.first == l.first) {
                            check = false;
                            m.second += l.second;
                        }
                    }
                    if (check)
                        ingredients.push_back(l);
                }
                
                bool check_can_make = true;
                for (auto l : ingredients) {
                    map<string, map<int, int>>::iterator iter = usable_meal.find(l.first);
                    if (iter == usable_meal.end()) {
                        check_can_make = false;
                        break;
                    }

                    for (map<int, int>::iterator m = iter->second.begin(); m != iter->second.end(); ++m) {
                        if (l.second > m->second) {
                            l.second -= m->second;
                            m->second = 0;
                        }
                        else {
                            m->second -= l.second;
                            l.second = 0;
                        }
                    }
                    if (l.second != 0) {
                        check_can_make = false;
                        break;
                    }
                }

                if (check_can_make) {
                    newMeal.push_back(i->getName());
                    newMeal.push_back(j->getName());
                    newMeal.push_back(k->getName());

                    double Satisfaction = 0;
                    double Expiration = 0;
                    Satisfaction += i->getScore();
                    Satisfaction += j->getScore();
                    Satisfaction += k->getScore();
                    for(auto l : usable_meal) {
                        for(auto m : l.second) {
                            Expiration += m.second * m.first;
                        }
                    }
                    
                    courses.push_back(make_pair(newMeal, make_pair(Satisfaction, Expiration)));
                }
            }
        }
    }

    double Satisfaction_MAX = 0;
    double Expiration_MAX = 0;
    
    for (auto i : courses) {
        if (Satisfaction_MAX < i.second.first)
            Satisfaction_MAX = i.second.first;
        if (Expiration_MAX < i.second.second)
            Expiration_MAX = i.second.second;
    }

    for (auto j=0; j<3; j++) {
        double MAX_score = 0;
        vector<Course>::iterator temp_iter = courses.end();
        for (auto i = courses.begin(); i != courses.end(); ++i) {
            double temp1 = i->second.first / Satisfaction_MAX;
            double temp2 = i->second.second / Expiration_MAX;
            if (MAX_score < temp1 + temp2) {
                MAX_score = temp1 + temp2;
                temp_iter = i;
            }
        }
        if (temp_iter == courses.end()) {
            break;
        }
        else {
            best.push_back(make_pair(temp_iter->first, temp_iter->second));
            courses.erase(temp_iter);
        }
    }

    if (best.empty()) {
        cout << "There are no possible course meal." << endl;
    }
    else {
        for (auto i : best) {
            double temp1 = i.second.first / Satisfaction_MAX;
            double temp2 = i.second.second / Expiration_MAX;
            cout << "1. " << i.first.at(0) << " 2. " << i.first.at(1) << " 3. " << i.first.at(2)
            << " / total score sum : " << temp1 + temp2
            << " (" << temp1 << " / " << temp2 << ")" << endl;
        }
    }
}

/**
 * base function of the smart refrigerator
 * User should type a number between 1 and 4 to call a function of the smart
 * regrigerator Then, this function utilizes the other functions of the smart
 * refrigerator as the user's input Below is the list of options that menuSelect
 * should support 1) display 2) insert food 2-1) insert a list of foods from a
 * text file (.txt) 2-2) insert a food directly 3) pop food 4) add recipe 5)
 * show recipe 6) recommend meals 7) end program Unless the user ends the
 * program ( 7) end program ), this function does not terminate and keeps
 * requiring the user to type a number
 * @return 0 when the user ends the program by typing else 1
 */

int SmartRefrigerator::menuSelect()
{
    int i;
    while (1)
    {
        cout << "************************************************************" << endl;
        cout << "************************   Menu   **************************" << endl;
        cout << "************************************************************" << endl;

        cout << "1. display" << endl
             << "2. insert food" << endl
             << "3. pop food" << endl
             << "4. add recipe" << endl
             << "5. show recipe" << endl
             << "6. recommend meal courses" << endl
             << "7. end program" << endl
             << "Select the function you want : ";
        cin >> i;
        if (i < 1 || i > 7){
            cout << "Type an integer from 1 to 7!" << endl;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        else {break;}
    }
    if (i == 7)
        return 0;
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
            case 4:
                addRecipeFromFile();
                break;
            case 5:
                showRecipe();
                break;
            case 6:
                recommendMealCourses();
                break;
        }
        return 1;
    }
}