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
    /* === TODO: implement ==== */
    // stack FoodInFridge object in "shelves"
    // 1. find location to stack
    // 2. if there is no right place, return false
    // 3. if shelf is full, make new shelf (until reaching maxHeight)

    // assume that shelves is stack // higher height -> back of "shelves"
    int x, y;
    if(!shelves.empty()){
        auto upper_shelf = shelves.back();
        auto list_on_shelf = upper_shelf.vec;
        auto last_food_on_shelf = list_on_shelf.back();
        auto position = last_food_on_shelf->getPos();
        auto size = last_food_on_shelf->getSize();
        // position for new Food
        x = position.first + size.first;
        y = position.second;
    }
    else{
        // if shelf is empty, push 1st shelf
        Shelf first_shelf(0);
        shelves.push_back(first_shelf);
        x = 0;
        y = 0;
    }

    if(y + foodSize.second > 19){
        // this case stacking food is impossible
        return false;
    }

    if(x + foodSize.first > 29){
        int height = y + maxHeight;
        if(height <= 19){
            x = 0;
            y = height;
            if(y + foodSize.second > 19){
                return false;
            }
            else{
                Shelf new_shelf(height);
                FoodPtr stack_food = new FoodInFridge(Food(name, foodSize, exp), x, y);
                new_shelf.vec.push_back(stack_food);
                shelves.push_back(new_shelf);
                cout << "Inserting " << name << " into x: " << x << ", y: " << y << endl; 
            }
        }
        else{
            return false;
        }
    }
    else{
        FoodPtr stack_food = new FoodInFridge(Food(name, foodSize, exp), x, y);
        shelves.back().vec.push_back(stack_food);
        cout << "Inserting " << name << " into x: " << x << ", y: " << y << endl;
    }

    // stack FoodInFridge object in "FoodList"
    // if food is exist in foodList
    if(foodList.find(name) != foodList.end()){
        // update value of foodList[name]
        auto food_vec = foodList[name];
        // x, y는 냉장고에 들어갈 위치
        FoodPtr stack_food = new FoodInFridge(Food(name,foodSize,exp),x,y);
        food_vec.push_back(stack_food);
        // 여기서 여러개의 FoodPtr이 안들어감!!
    }
    else{
        FoodPtr stack_food = new FoodInFridge(Food(name,foodSize,exp),x,y);
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
    /**
     * ===============================================
     * ======== TODO: Implement this function ========
     * ===============================================
     */
    return false;
}
