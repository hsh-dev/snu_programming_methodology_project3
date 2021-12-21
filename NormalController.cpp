#include "NormalController.h"
#include "Controller.h"
#include "Food.h"


/**
 * Store a given food object into the refrigerator. 
 * 
 * This function should follows the 'Modified Shelf First Fit (Shelf-FF) algorithm, which is given in the PPT slide.
 * Your implemented algorithm should find the proper position of the new food object.
 * The new food object should be initialized in the heap memory and referenced from both food_list and shelves.
 * 
 * @param name The name of the food to be stored
 * @param size width and height of the food
 * @param exp the expire date of the food
 * @return true if inserting the food for both food_list and shelves has been succeed
 */
bool NormalController::stackFood(const string name, intPair foodSize, int exp)
{
    // TODO
    // stack FoodInFridge object in "shelves"
    // 1. find location to stack
    // 2. if there is no right place, return false
    // 3. if shelf is full, make new shelf (until reaching maxHeight)

    if(shelves.empty()){
        Shelf first_shelf(0);
        shelves.push_back(first_shelf);
    }
    
    int x, y, level;
    // "x" and "y" is position in refrigerator
    // "level" is index of shelf // level = 0 means first floor
    // "space" is initialize with 0 and if there is food, it is filled with 1
    vector<vector<int>> shelf_space;
    for(auto it : shelves){
        vector<int> space(30);  // space for each shelf
        auto list = it.vec;
        for(auto food_it : list){
            auto pos = food_it->getPos();
            auto size = food_it->getSize();
            for(auto idx = pos.first ; idx < pos.first+size.first ; idx++){
                space[idx] = 1;
            }
        }
        shelf_space.push_back(space);
    }
    vector<int> shelf_height;   // save height of each shelf
    for(auto it : shelves){
        shelf_height.push_back(it.height);
    }

    bool stack_or_not = false; // when stack_or_not became true, it means that it can stack somewhere
    for(auto k = 0 ; k < shelf_space.size() ; k++){
        auto space_list = shelf_space[k];
        for(auto i = 0 ; i < space_list.size() ; i++){
            bool possible_or_not = true;
            if(space_list[i] == 0){
                for(auto j = i ; j < i+foodSize.first ; j++){
                    if(space_list[j] == 1 || j > 29){
                        possible_or_not = false;
                    }
                }
                if (possible_or_not){
                    // if location is not on the top
                    if(k != shelf_space.size()-1){
                        if(shelf_height[k] + foodSize.second <= shelf_height[k+1]){
                            stack_or_not = true;
                            x = i;
                            y = shelf_height[k];
                            level = k;
                            break;
                        }
                    }
                    else{   // if location is on the top
                        if(shelf_height[k] + foodSize.second <= 20){
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
        if(stack_or_not){
            break;
        }
    }
    // case if food can not be stacked in existing shelf
    // make new shelf if it is possible
    FoodPtr stack_food;
    if(!stack_or_not){
        int h = shelves.back().height;
        // in normal controller, maxHeight is added to new shelf 
        h = h + maxHeight;
        if(h + foodSize.second <= 20){
            x = 0;
            y = h;
            Shelf new_shelf(h);
            stack_food = new FoodInFridge(Food(name, foodSize, exp), x, y);
            new_shelf.vec.push_back(stack_food);
            shelves.push_back(new_shelf);
            cout << "Inserting " << name << " into x: " << x << ", y: " << y << endl;
        }
        else{
            cout << "The foods cannot be inserted" << endl;
            return false;
        }
    }
    else{
        stack_food = new FoodInFridge(Food(name, foodSize, exp), x, y);
        shelves[level].vec.push_back(stack_food);
        cout << "Inserting " << name << " into x: " << x << ", y: " << y << endl;
    }

    // stack FoodInFridge object in "FoodList"
    // if food is exist in foodList
    if(foodList.find(name) != foodList.end()){
        // update value of foodList[name]
        auto food_vec = foodList[name];
        // x, y is position in refrigerator
        food_vec.push_back(stack_food);
        foodList[name] = food_vec;
    }
    else{
        vector<FoodPtr> food_vec;
        food_vec.push_back(stack_food);
        foodList.insert({name,food_vec});
    }

    return true;
}


/**
 * Pop a food with shortest expire date from both foodList and shelves.
 * This function does not return the object.
 * The pop process should follows the algorithm in PPT slide.
 * 
 * @param food_name a name of food to be popped
 * @return 
 */
bool NormalController::popFood(const string food_name)
{
    // TODO
    // Normal controller only need pop function
    // need to delete FoodPtr
    if(foodList[food_name].empty()){
        cout << "There's no food that you want!" << endl;
        // if there is no food_name in foodList, return false
        return false;
    }
    auto pop_food = minExp_from_right(food_name);
    auto food_ptr = *pop_food;
    auto pop_exp = food_ptr->getExp();
    auto pop_size = food_ptr->getSize();
    auto pos = food_ptr->getPos();
    
    // delete in shelves first // searching target food from top to bottom, right to left
    bool erase_or_not = false;
    for(auto level = shelves.size()-1 ; level >= 0 ; level--){
        auto food_list = shelves[level].vec;
        for(int i = food_list.size()-1 ; i >= 0 ; i--){
            if(pos.first == food_list[i]->getPos().first && pos.second == food_list[i]->getPos().second){
                shelves[level].vec.erase(shelves[level].vec.begin()+i);
                erase_or_not = true;
                break;
            }
        }
        if(erase_or_not){
            break;
        }
    }

    auto food_vec = foodList[food_name];
    foodList.erase(food_name);  // initialize foodList
    for(auto i = 0 ; i < food_vec.size() ; i++){
        if (pos.first == food_vec[i]->getPos().first && pos.second == food_vec[i]->getPos().second)
        {
            // deleting target foodPtr
            food_vec[i] = nullptr;
            food_vec.erase(food_vec.begin()+i);
            // printing out the message
            cout << "position to pop: " << "x: " << pos.first << " y: " << pos.second << endl;
            cout << "A food " << food_name << " with expire date " << pop_exp << " has been popped" << endl;
        }
    }
    // if food_vec is not empty, insert food_vec to foodList again
    if(!food_vec.empty()){
        foodList.insert({food_name, food_vec});
    }

    return true;
}
