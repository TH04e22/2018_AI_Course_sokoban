#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "point.h"
using namespace std;

struct box {
    int barrier;
    Point position;
public:
    box( const int& bar, const Point& pos ) {
        barrier = bar;
        position = pos;
    }
};

inline bool operator<( const box& a, const box& b ) {
    return a.barrier < b.barrier;
}

int main( int argc, char** argv ) {
    Point p( 1, 2 );
    map< Point, int > boxes;
    boxes[p] = 4;
    p.setX(3); p.setY(4);
    boxes[p] = 1;
    p.setX(6); p.setY(2);
    boxes[p] = 2;
    map< Point, int >::iterator it;
    vector<box> boxVector;
    for( it = boxes.begin(); it != boxes.end(); it++ ) {
        it->second++;
        box temp(it->second,it->first);
        boxVector.push_back(temp);
    }

    sort( boxVector.begin(), boxVector.end());
    for( const box &b:boxVector) {
        cout << "Priority: " << b.barrier << " Position: ( "
            << b.position.getX() << ", " << b.position.getY() << " )" <<endl;
    }
}