#ifndef POINT_H
#define POINT_H
#include <initializer_list>
class Point{
public:
	Point( int x = 0, int y = 0 ) {
		setX( x ); setY( y );
	}
	
	Point( const Point& b ) {
		setX(b.getX());
		setY(b.getY());
	}

	Point( std::initializer_list<int> list ) {
		setX(*list.begin());
		setY(*(list.begin()+1));
	}
    ~Point() {}
	inline void setX( int x ) { this->x = x; }
	inline void setY( int y ) { this->y = y; }
	inline int getX() const { return x; }
	inline int getY() const { return y; }
	bool operator < ( const Point &b ) const {
		if( this->getX() < b.getX() )
			return true;
		else if( this->getX() == b.getX() ) {
			if( this->getY() < b.getY() )
				return true;
			else
				return false;
		} else
			return false;
	}

	Point& operator=( const Point &b ) {
		setX(b.getX());
		setY(b.getY());
		return *this;
	}

	bool operator==( const Point &b ) const {
		if( (this->getX() == b.getX() ) && (this->getY() == b.getY() ) )
			return true;
		return false;
	}

	bool operator!=( const Point &b ) const {
		if( (*this) == b  )
			return false;
		return true;
	}
private:
	int x;
	int y;
};
#endif