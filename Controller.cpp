#include "Controller.h"
#include "Food.h"

/**
 * A constructor of the Controller class
 * 
 * The food location is rightEnd of the shelf
 *
 * @param size_ A size of refrigerator
 * @param foodList_ A foodList container
 * @return true if inserting succeed.
 */
Controller::Controller(intPair size_, foodListType &foodList_) : size(size_), foodList(foodList_)
{
    storageGrid = new bool *[size.second];
    for (int i = 0; i < size.second; i++)
        storageGrid[i] = new bool[size.first]{0};
}

/**
 * Search the food with shortest expire date from foodList and return its iterator.
 * 
 * @param food_name a name of food to be searched.
 * @return an iterator of the food with shortest expire date.
 */
vector<FoodPtr>::iterator Controller::findMinExpFood(const string food_name)
{
    vector<FoodPtr> &v = foodList[food_name];
    // get idx with minimum exp date
    auto comp = [](const FoodPtr a, const FoodPtr b)
    {
        return (a->getExp() < b->getExp());
    };
    return min_element(v.begin(), v.end(), comp);
}

/**
 * Set element of storageGrid in the given range to true
 *
 * @param x The x-axis position of object
 * @param y The y-axis position of object
 * @param w The width of the object
 * @param h The height of the object
 * @return 
 */
void Controller::drawFoodsInStorageGrid(int x, int y, int w, int h)
{
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
            storageGrid[y + j][x + i] = true;
    }
}

/**
 * Print out the storage state of the refrigerator
 * 
 * @param
 * @return 
 */
void Controller::display()
{
    // Reset storageGrid to false
    for (int i = 0; i < size.second; i++)
    {
        for (int j = 0; j < size.first; j++)
            storageGrid[i][j] = false;
    }

    // Fill out the areas that the object exists
    for (auto it : foodList)
    {
        for (auto fnp : it.second)
            drawFoodsInStorageGrid(fnp->getPos().first, fnp->getPos().second, fnp->getSize().first, fnp->getSize().second);
    }

    // Printout current storage states
    cout << "****************Your current storage state*****************" << endl;
    cout << "width: " << size.first << ", height: " << size.second << endl;
    for (int i = 0; i < size.second; i++)
    {
        for (int j = 0; j < size.first; j++)
        {
            if (storageGrid[size.second - i - 1][j] == false)
                cout << "ㅁ";
            else
                cout << "\u2B1C";
        }
        cout << endl;
    }
    cout << "****************Your current storage state*****************" << endl;
}