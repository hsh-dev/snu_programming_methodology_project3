#include "SmartController.h"
#include "Controller.h"
#include "Food.h"

/**
 * Get maximum height among the foods in this shelf
 * 
 * @return maximum height of the foods.
 */
int SmartController::maxHeight(Shelf &shelf)
{
    if (shelf.vec.empty())
        return 0;
    auto comp = [](const FoodPtr a, const FoodPtr b)
    {
        return (a->getSize().second < b->getSize().second);
    };
    auto it_shelf = max_element(shelf.vec.begin(), shelf.vec.end(), comp);

    return (*it_shelf)->getSize().second;
}

/**
 * Store a given food object into the refrigerator
 * 
 * This function should follows the 'Shelf First Fit (Shelf-FF) algorithm, which is given in the PPT slide.
 * Your implemented algorithm should find the proper position of the new food.
 * Also, the given food object may in the stack memory. You should save it in the heap memory and reference
 * it from both food_list and shelves.
 * 
 * @param name The name of the food to be stored
 * @param size width and height of the food
 * @param exp the expire date of the food
 * @return true if inserting the food for both food_list and shelves has been succeed
 */
bool SmartController::stackFood(const string name, intPair foodSize, int exp)
{   
    // TODO
    if (shelves.empty())
    {
        Shelf first_shelf(0);
        shelves.push_back(first_shelf);
    }

    int x, y, level;
    vector<vector<int>> shelf_space;    // space for each shelf
    for (auto it : shelves)
    {
        vector<int> space(30);
        auto list = it.vec;
        for (auto food_it : list)
        {
            auto pos = food_it->getPos();
            auto size = food_it->getSize();
            for (auto idx = pos.first; idx < pos.first + size.first; idx++)
            {
                space[idx] = 1;
            }
        }
        shelf_space.push_back(space);
    }

    vector<int> shelf_height; // save height of each shelf
    for (auto it : shelves)
    {
        shelf_height.push_back(it.height);
    }

    bool stack_or_not = false; // when stack_or_not became true, it means that it can stack somewhere
    for (auto k = 0; k < shelf_space.size(); k++)
    {
        auto space_list = shelf_space[k];
        for (auto i = 0; i < space_list.size(); i++)
        {
            bool possible_or_not = true;
            if (space_list[i] == 0)
            {
                for (auto j = i; j < i + foodSize.first; j++)
                {
                    if (space_list[j] == 1 || j > 29)
                    {
                        possible_or_not = false;
                    }
                }
                if (possible_or_not)
                {
                    // if location is not on the top
                    if (k != shelf_space.size() - 1)
                    {
                        if (shelf_height[k] + foodSize.second <= shelf_height[k + 1])
                        {
                            stack_or_not = true;
                            x = i;
                            y = shelf_height[k];
                            level = k;
                            break;
                        }
                    }
                    else
                    { // if location is on the top
                        if (shelf_height[k] + foodSize.second <= 20)
                        {
                            stack_or_not = true;
                            x = i;
                            y = shelf_height[k];
                            level = k;
                            break;
                        }
                    }
                }
            }
        }
        if (stack_or_not)
        {
            break;
        }
    }

    // insert on shelves
    FoodPtr stack_food;
    if (!stack_or_not)
    {
        int h = shelves.back().height;
        // in smart controller, add max height on that shelf
        h = h + maxHeight(shelves.back());
        if (h + foodSize.second <= 20)
        {
            x = 0;
            y = h;
            Shelf new_shelf(h);
            stack_food = new FoodInFridge(Food(name, foodSize, exp), x, y);
            new_shelf.vec.push_back(stack_food);
            shelves.push_back(new_shelf);
            cout << "Inserting " << name << " into x: " << x << ", y: " << y << endl;
        }
        else
        {
            cout << "The foods cannot be inserted" << endl;
            return false;
        }
    }
    else
    {
        stack_food = new FoodInFridge(Food(name, foodSize, exp), x, y);
        shelves[level].vec.push_back(stack_food);
        cout << "Inserting " << name << " into x: " << x << ", y: " << y << endl;
    }

    // stack FoodInFridge object in "FoodList"
    // if food is exist in foodList
    if (foodList.find(name) != foodList.end())
    {
        // update value of foodList[name]
        auto food_vec = foodList[name];
        // x, y is position in refrigerator
        food_vec.push_back(stack_food);
        foodList[name] = food_vec;
    }
    else
    {
        vector<FoodPtr> food_vec;
        food_vec.push_back(stack_food);
        foodList.insert({name, food_vec});
    }

    return true;
}

/**
 * Pop food with shortest expire date from both foodList and shelves.
 * This function does not return the object.
 * The pop process should follows the algorithm in PPT slide.
 * 
 * @param food_name a name of food to be popped
 * @return 
 */
bool SmartController::popFood(const string food_name) // void
{

    // TODO
    if (foodList[food_name].empty())
    {
        cout << "There's no food that you want!" << endl;
        // if there is no food_name in foodList, return false
        return false;
    }
    auto pop_food = minExp_from_right(food_name);
    auto food_ptr = *pop_food;
    auto pop_exp = food_ptr->getExp();
    auto pop_size = food_ptr->getSize();
    auto pos = food_ptr->getPos();
    int deleted_level(0);

    // delete in shelves first // searching target food from top to bottom, right to left
    bool erase_or_not = false;
    for (auto level = shelves.size() - 1; level >= 0; level--)
    {
        auto food_list = shelves[level].vec;
        for (int i = food_list.size() - 1; i >= 0; i--)
        {
            if (pos.first == food_list[i]->getPos().first && pos.second == food_list[i]->getPos().second)
            {
                shelves[level].vec.erase(shelves[level].vec.begin() + i);
                erase_or_not = true;
                deleted_level = level;

                for(int j = i ; j < shelves[level].vec.size() ; j++){
                    auto temp_pos = shelves[level].vec[j] -> getPos();
                    intPair new_pos = make_pair(temp_pos.first-pop_size.first,temp_pos.second);
                    shelves[level].vec[j] -> setPos(new_pos);
                }
                break;
            }
        }
        if (erase_or_not)
        {
            break;
        }
    }

    auto food_vec = foodList[food_name];
    foodList.erase(food_name); // initialize foodList
    for (auto i = 0; i < food_vec.size(); i++)
    {
        if (pos.first == food_vec[i]->getPos().first && pos.second == food_vec[i]->getPos().second)
        {
            // deleting target foodPtr
            food_vec[i] = nullptr;
            food_vec.erase(food_vec.begin() + i);
            // printing out the message
            cout << "position to pop: "
                 << "x: " << pos.first << " y: " << pos.second << endl;
            cout << "A food " << food_name << " with expire date " << pop_exp << " has been popped" << endl;
        }
    }
    // if food_vec is not empty, insert food_vec to foodList again
    if (!food_vec.empty())
    {
        foodList.insert({food_name, food_vec});
    }

    // update shelf height
    bool need_update = false;
    int gap(0);
    if (deleted_level != shelves.size() - 1)
    {
        if(maxHeight(shelves[deleted_level]) != shelves[deleted_level+1].height-shelves[deleted_level].height){
            need_update = true;
            gap = shelves[deleted_level + 1].height - shelves[deleted_level].height - maxHeight(shelves[deleted_level]);
            shelves[deleted_level+1].height = shelves[deleted_level+1].height - gap;
        }
    }
    // else, no reason to update height

    // height update when there is gap
    if(need_update){
        for(auto i = deleted_level+1 ; i < shelves.size() ; i++){
            for (auto j = 0; j < shelves[i].vec.size(); j++)
            {
                auto temp_pos = shelves[i].vec[j]->getPos();
                intPair new_pos = make_pair(temp_pos.first,temp_pos.second-gap);
                shelves[i].vec[j]->setPos(new_pos);
            }
        }
    }

    // case when shelf is empty
    if(shelves[deleted_level].vec.empty()){
        shelves.erase(shelves.begin()+deleted_level);
    }

    return true;
}