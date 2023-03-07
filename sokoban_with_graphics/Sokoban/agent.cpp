#include "agent.h"
#include "Windows.h"
using namespace std;
static const int UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, RESTART = 4, LEAVE = 5;
static const vector<Point> possibleAction = {{0,-1},{0,1},{-1,0},{1,0}};
agent::agent( vector<vector<int>> maze,vector<Point> target, Point agentPos ) {
    setConfig( maze, target, agentPos );
}

agent::agent( QVector<QVector<int>> maze, QVector<QPoint> target, QPoint agentPos ) {
    this->mapSize.setY( maze.size() );
    this->mapSize.setX( maze[0].size() );
    this->agentPos.setX(agentPos.x()); this->agentPos.setY(agentPos.y());
    for ( const QPoint &p: target ) {
        Point temp(p.x(),p.y());
        this->targetVector.push_back(temp);
    }

    agentView.resize( mapSize.getY() );
    for( vector<int> &v : agentView) {
        v.resize( mapSize.getX() );
    }

    for( int i = 0; i < agentView.size(); i++ ) { // Initial box position
        for ( int j = 0; j < agentView[i].size(); j++ )
            agentView[i][j] = maze[i][j];
    }

    for( int i = 0; i < agentView.size(); i++ ) { // Initial box position
        for ( int j = 0; j < agentView[i].size(); j++ )
            if( agentView[i][j] == 2 ) {
                boxVector.push_back(Point(j,i));
                agentView[i][j] = 0;
            }
    }
    this->boxView = this->agentView;
}

void agent::setConfig( std::vector<std::vector<int>> maze, std::vector<Point> targetVector, Point agentPos ) {
	this->mapSize.setY( maze.size() );
    this->mapSize.setX( maze[0].size() );
    this->agentPos = agentPos;
    this->targetVector = targetVector;
    this->agentView = maze;
	for( int i = 0; i < agentView.size(); i++ ) { // Initial box position
		for ( int j = 0; j < agentView[i].size(); j++ )
			if( agentView[i][j] == 2 ) {
				boxVector.push_back(Point(j,i));
				agentView[i][j] = 0;
			}
	}
	this->boxView = this->agentView;
}

vector<vector<int>> agent::getAgentView() const {
	return agentView;
}

vector<vector<int>> agent::getBoxView() const {
	return boxView;
}

void agent::edgeScan() { // Use breath first search to scan the maze
	vector<Point> nextAction = {{0,-1},{1,0},{0,1},{-1,0}}; // Possible action
    set<Point> explored; // Explored state
	queue<Point> frontier; // breath first search frontier
	for ( Point &p: targetVector ) { // explicit annotate target
		if( boxView[p.getY()][p.getX()] != 2 && boxView[p.getY()][p.getX()] != 1 )
			boxView[p.getY()][p.getX()] = 5;
		explored.insert(p); // insert targets into explored set
	}
	frontier.push(agentPos); // begin at agent position
	explored.insert(agentPos); // add begin position into frontier
	Point currentState, nextState;
	while( !frontier.empty() ) {
		currentState = frontier.front();
		frontier.pop();
		for( Point &p: nextAction ) {
			nextState.setX(p.getX() + currentState.getX());
			nextState.setY(p.getY() + currentState.getY());
			if((boxView[nextState.getY()][nextState.getX()] == 0 || boxView[nextState.getY()][nextState.getX()] == 2 ) 
				&& explored.find(nextState) == explored.end() ) 
			{
				frontier.push(nextState);
				explored.insert(nextState);
			}
		}
		for( int it = 0; it < nextAction.size(); ++it ) {
			if( boxView[currentState.getY()+nextAction[it].getY()][currentState.getX()+nextAction[it].getX()] == 3 // check L region
			&& boxView[currentState.getY()+nextAction[(it+1)%4].getY()][currentState.getX() + nextAction[(it+1)%4].getX()] == 3 ) {
				if( boxView[currentState.getY()][currentState.getX()] == 0 || boxView[currentState.getY()][currentState.getX()] == 1) {
					boxView[currentState.getY()][currentState.getX()] = 8;
					break;
				} else if (boxView[currentState.getY()][currentState.getX()] == 2){
					std::cout << "The game has no solutions" << std::endl;
					break;
				}
			} 
		}

		for( int it = 0; it != nextAction.size(); ++it ) {
			if( boxView[currentState.getY()+nextAction[it].getY()][currentState.getX()+nextAction[it].getX()] == 3 ) { // Only one side has wall
				Point w_side(nextAction[it].getX(), nextAction[it].getY()); // wall side
				Point a_side(nextAction[(it+1)%4].getX(),nextAction[(it+1)%4].getY()); // forward side
				Point b_side(nextAction[(it+3)%4].getX(),nextAction[(it+3)%4].getY()); // backward side
				
				bool has_open = false;
				int x = currentState.getX(), y = currentState.getY();
				while( boxView[y][x] != 3 && !has_open ) {
					if( boxView[y+w_side.getY()][x+w_side.getX()] == 0 || boxView[y+a_side.getY()][x+a_side.getX()] == 5|| boxView[y+w_side.getY()][x+w_side.getX()] == 8|| boxView[y+w_side.getY()][x+w_side.getX()] == 2 ) {
						has_open = true;
						break;
					}
					x += a_side.getX();
					y += a_side.getY();
				}

				x = currentState.getX(); y = currentState.getY();
				while( boxView[y][x] != 3 && !has_open ) {
					if( boxView[y+w_side.getY()][x+w_side.getX()] == 0 || boxView[y+b_side.getY()][x+b_side.getX()] == 5|| boxView[y+w_side.getY()][x+w_side.getX()] == 8||boxView[y+w_side.getY()][x+w_side.getX()] == 2 ) {
						has_open = true;
						break;
					}
					x += b_side.getX();
					y += b_side.getY();
				}
				if( !has_open )
					boxView[currentState.getY()][currentState.getX()] = 8;
			}
		}
	}

	for ( const Point &p: targetVector ) {
		if(boxView[p.getY()][p.getX()] == 5 )
			boxView[p.getY()][p.getX()] = 0;
	}
}

void agent::showMaze( std::vector<std::vector<int>> View ){ // print the maze
	for ( Point &p: targetVector ) {
		if( View[p.getY()][p.getX()] != 2 && View[p.getY()][p.getX()] != 1 )
			View[p.getY()][p.getX()] = 5;
	}
	for ( vector<int> &temp: View ) {
		for ( int &number: temp ) {
			switch(number) {
				case 0: // ROAD
					cout << "  ";
					break;
				case 4: // BLACK
					cout << " .";
					break;
				case 1: // AGENT
					cout << " !";
					break;
				case 2: // BOX
					cout << " ^";
					break;
				case 3: // WALL
					cout << " #";
					break;
				case 5: // TARGET
					cout << " @";
					break;
                case 8: // Box can't move
                    cout << " X";
                    break;
				default:
					break;
			}
		}
		cout << endl;
	}
	for ( Point &p: targetVector ) {
		if(View[p.getY()][p.getX()] == 5 )
			View[p.getY()][p.getX()] = 0;
	}
}


void agent::printTargetPos() const {
	cout << "------------Target Position------------" << endl;
	for( const Point &p: targetVector )
		cout << "x: " << p.getX() << " y: " << p.getY() << endl;
	cout << endl;
}

void agent::printBoxPos() const {
	cout << "------------Box Position------------" << endl;
	for( const Point &p: boxVector )
		cout << "x: " << p.getX() << " y: " << p.getY() << endl;
	cout << endl;
}

float agent::manhattanDistance( Point src, Point dst ) {
	return static_cast<float>(abs(dst.getX() - src.getX()) + abs(dst.getY() - src.getY()));
}

float agent::euclideanDistance( Point src, Point dst ) {
	float x_distance = static_cast<float>(dst.getX() - src.getX());
	float y_distance = static_cast<float>(dst.getY() - src.getY());
	return sqrt( x_distance*x_distance+y_distance*y_distance);
}

int agent::getAction() {
	if( !actionStack.empty() ) {
		int action = actionStack.front();
		actionStack.pop_front();
		return action;
	} else {
		return -1;
	}
}

bool agent::planAction() {
	edgeScan();
	vector<target> aims;
	map<Point, box> cargos;
	while( !actionStack.empty() ) // clean the action stack
		actionStack.pop_back();

	for ( int i = 0; i < targetVector.size(); i++ ) {
		target temp( 0, targetVector[i] );
		aims.push_back( temp );
	}

	for ( int i = 0; i < boxVector.size(); i++ ) {
		set<Point> new_explored;
		box temp( 0, boxVector[i], new_explored );
		cargos[temp.position]= temp; 
	}
	evaluateTarget( aims, cargos );
	evaluateBox( aims[0], cargos );
	
	#ifdef DEBUG
	cout << "TARGET:\n";
	for ( const target& t: aims ) {
		cout << "Priority: " << t.barrier << " Position: ( "
			<< t.position.getX() << " , " << t.position.getY() << " )" << endl;
	}
	cout << "BOXES:\n";
	for ( map<Point, box>::iterator it = cargos.begin(); it != cargos.end(); it++  ) {
		cout << "Priority: " << it->second.barrier << " Position: ( "
			<< it->first.getX() << " , " << it->first.getY() << " )" << endl;
	}
	#endif
	set<Point> second;
	state first;
	bool has_sol = boxTurn( aims, cargos, agentPos, true, first, second  );
	return has_sol;
	return true;
}

bool agent::boxTurn( vector<target> aims, map<Point, box> boxes, Point agentPos, bool plan, state& agentWantState ,set<Point> agentExplored  ) {
	#ifdef DEBUG
	Sleep(500);
	cout << "************Box Turn**************" << endl;
	agentView[agentPos.getY()][agentPos.getX()] = 1;
	for( const target& t: aims ) {
		agentView[t.position.getY()][t.position.getX()] = 5;
	}
	for ( map<Point, box>::iterator it = boxes.begin(); it != boxes.end(); it++  ) {
		agentView[it->first.getY()][it->first.getX()] = 2;
	}
	showMaze( agentView );
	agentView[agentPos.getY()][agentPos.getX()] = 0;
	for( const target& t: aims ) {
		agentView[t.position.getY()][t.position.getX()] = 0;
	}
	for ( map<Point, box>::iterator it = boxes.begin(); it != boxes.end(); it++  ) {
		agentView[it->first.getY()][it->first.getX()] = 0;
	}
	cout << "TARGET:\n";
	for ( const target& t: aims ) {
		cout << "Priority: " << t.barrier << " Position: ( "
			<< t.position.getX() << " , " << t.position.getY() << " )" << endl;
	}

	cout << "BOXES:\n";
	for ( map<Point, box>::iterator it = boxes.begin(); it != boxes.end(); it++  ) {
		cout << "Priority: " << it->second.barrier << " Position: ( "
			<< it->first.getX() << " , " << it->first.getY() << " )" << endl;
	}
	#endif
	/* implement */
    agentExplored.clear();
	if( plan ) {
		if ( aims.empty() ) // find the solution
			return true;
		evaluateTarget( aims, boxes );
		sort( aims.begin(), aims.end() );
	}

	for ( const target& major : aims) {
		if( plan ) 
			evaluateBox( major, boxes );
		map< Point, box >::iterator it;
		vector<box> boxList;
		for( it = boxes.begin(); it != boxes.end(); it++ ) {
			if ( it->second.barrier > -1 ) {
        		boxList.push_back(it->second);
			}
    	}
		sort( boxList.begin(),boxList.end());
		for ( int i = 0; i < boxList.size(); i++ ) {
			// aging
			for ( int j = 0; j < i; j++ ) {
				boxes[boxList[j].position].barrier = boxList[i].barrier + 1;	
			}

			vector<state> nextState;
			Point nextPosition;
			Point actionPosition;
			boxes[boxList[i].position].explored.insert(boxList[i].position);
			for ( int j = 0; j < possibleAction.size(); j++ ) {
				nextPosition.setX( boxList[i].position.getX() + possibleAction[j].getX()); nextPosition.setY(boxList[i].position.getY() + possibleAction[j].getY() );
				actionPosition.setX( boxList[i].position.getX() - possibleAction[j].getX() ); actionPosition.setY(boxList[i].position.getY() - possibleAction[j].getY());
				if ( boxView[nextPosition.getY()][nextPosition.getX()] < 2 &&
				     agentView[actionPosition.getY()][actionPosition.getX()] < 2 &&
					 boxes.find(actionPosition) == boxes.end() &&
					 boxes[boxList[i].position].explored.find(nextPosition) == boxes[boxList[i].position].explored.end() &&
					 boxes.find( nextPosition ) == boxes.end() ) {
					state temp( manhattanDistance( nextPosition, major.position), boxList[i].position, major.position, j );
					nextState.push_back( temp );
				}
			}

			sort( nextState.begin(), nextState.end() );
			for ( state &s: nextState ) {
				Point nextPosition;
				nextPosition.setX( possibleAction[s.action].getX() + s.currentPosition.getX()); nextPosition.setY(possibleAction[s.action].getY() + s.currentPosition.getY());
				boxes[boxList[i].position].explored.insert(nextPosition);
				if ( agentTurn( aims, boxes, agentPos, s, agentExplored ) ) {
					return true;
				} else {
					boxes[boxList[i].position].explored.erase( nextPosition );
				}	
			}
		}
	}
	return false;
}

bool agent::agentTurn( vector<target> aims, map<Point, box> boxes, Point agentPos, state& agentWantState, set<Point> agentExplored ){
	#ifdef DEBUG
	/* show maze */
	Sleep(500);
	cout << "**************agent Turn***************" << endl;
	agentView[agentPos.getY()][agentPos.getX()] = 1;
	for( const target& t: aims ) {
		agentView[t.position.getY()][t.position.getX()] = 5;
	}
	for ( map<Point, box>::iterator it = boxes.begin(); it != boxes.end(); it++  ) {
		agentView[it->first.getY()][it->first.getX()] = 2;
	}
	showMaze( agentView );
	agentView[agentPos.getY()][agentPos.getX()] = 0;
	for( const target& t: aims ) {
		agentView[t.position.getY()][t.position.getX()] = 0;
	}
	for ( map<Point, box>::iterator it = boxes.begin(); it != boxes.end(); it++  ) {
		agentView[it->first.getY()][it->first.getX()] = 0;
	}
	cout << "TARGET:\n";
	for ( const target& t: aims ) {
		cout << "Priority: " << t.barrier << " Position: ( "
			<< t.position.getX() << " , " << t.position.getY() << " )" << endl;
	}
	cout << "WANTED STATE:\n";
	cout << "Wanted Action: " << agentWantState.action << " Heuristic: " << agentWantState.heuristic << endl;
	cout << "boxCurrentPosition: ( " << agentWantState.currentPosition.getX() << " , "
		<< agentWantState.currentPosition.getY() << " )" << endl;
	cout << "boxWantPosition: ( " << agentWantState.boxWantPosition.getX() << " , "
		<< agentWantState.boxWantPosition.getY() << " )" << endl;
	cout << "agentWantPosition: ( " << agentWantState.currentPosition.getX() - possibleAction[agentWantState.action].getX() << " , "
		<< agentWantState.currentPosition.getY() - possibleAction[agentWantState.action].getY() << " )" << endl;
	cout << "AGENTPOS: ( " << agentPos.getX() << " , " << agentPos.getY() << " )" << endl; 
	#endif
	/* implement */
	Point dst( agentWantState.currentPosition.getX() - possibleAction[agentWantState.action].getX(), agentWantState.currentPosition.getY() - possibleAction[agentWantState.action].getY() );
	if( dst == agentPos ) {
		actionStack.push_back( agentWantState.action );
		agentPos.setX( agentPos.getX() + possibleAction[agentWantState.action].getX()); agentPos.setY(agentPos.getY() + possibleAction[agentWantState.action].getY());
		box temp = boxes[agentWantState.currentPosition];
		boxes.erase( agentWantState.currentPosition );
		dst.setX( agentWantState.currentPosition.getX() + possibleAction[agentWantState.action].getX() ); dst.setY( agentWantState.currentPosition.getY() + possibleAction[agentWantState.action].getY() );
		agentWantState.currentPosition = dst;
		temp.position = dst;
		boxes[dst] = temp;
		agentExplored.clear();
		bool match_target = false;
		if ( boxes.find(agentWantState.boxWantPosition) != boxes.end()  ) {
			for ( vector<target>::iterator a = aims.begin(); a != aims.end(); a++ ) {
				if( a->position == agentWantState.boxWantPosition ) {
					boxes[agentWantState.boxWantPosition].barrier = -1; // -1 means that the box doesn't need plan
					aims.erase( a );
					match_target = true;
					break;
				}
			}
		}

		if( (match_target ? boxTurn( aims, boxes, agentPos, true, agentWantState, agentExplored): boxTurn( aims, boxes, agentPos, false, agentWantState, agentExplored)) ) {
			return true;
		} else {
			actionStack.pop_back();
			return false;
		}
	} else {
		agentExplored.insert(agentPos);
		vector<state> nextState;
		for ( int i = 0; i < possibleAction.size(); i++ ) {
			Point temp( agentPos.getX() + possibleAction[i].getX(), agentPos.getY() + possibleAction[i].getY());
			if( boxes.find(temp) == boxes.end() &&
			    agentView[agentPos.getY() + possibleAction[i].getY()][agentPos.getX() + possibleAction[i].getX()] < 2 &&
				agentExplored.find(temp) == agentExplored.end() ) {
				state newState( manhattanDistance( temp, dst ),temp, agentWantState.boxWantPosition ,i);
				nextState.push_back( newState );
			}
		}
		
		if( nextState.empty() ) {
			return false;
		} else {
			sort( nextState.begin(), nextState.end() );
			for ( const state& s: nextState ) {
				actionStack.push_back( s.action );
				if( agentTurn( aims, boxes, s.currentPosition, agentWantState, agentExplored )){
					return true;
				} else {
					actionStack.pop_back();
				}
			}
			return false;
		}
	}
}

void agent::evaluateTarget( vector<target>& aims, map<Point, box>& boxes ) {
	int barrier;
	map<Point, box>::iterator it;
	for ( it = boxes.begin(); it != boxes.end(); it++ )
		boxView[it->first.getY()][it->first.getX()] = 2;
	
	for ( int i = 0; i < aims.size(); i++ ) {
		barrier = 0;
		for ( const Point &p: possibleAction ) {
			if(  agentView[aims[i].position.getY()+ p.getY()][aims[i].position.getX() + p.getX()] >= 2 )
				barrier++;
		}
		aims[i].barrier = barrier;
	}


	for ( it = boxes.begin(); it != boxes.end(); it++ )
		boxView[it->first.getY()][it->first.getX()] = 0;
}

void agent::evaluateBox( const target& aim, map<Point, box>& boxes ) {
	int barrier;
	map<Point, box>::iterator it;
	for ( it = boxes.begin(); it != boxes.end(); it++ )
		boxView[it->first.getY()][it->first.getX()] = 2;

	for ( it = boxes.begin(); it != boxes.end(); it++ ) {
		if ( it->second.barrier == -1 )
			continue;
		priority_queue<state,vector<state>> frontier;
		map<Point, Point> explored;
		state currentState( manhattanDistance(it->first,it->first) + euclideanDistance(it->first, aim.position) , it->first );
		explored[currentState.currentPosition] = currentState.currentPosition;
		frontier.push( currentState );
		while( !frontier.empty() ) {
			currentState = frontier.top();
			frontier.pop();

			if ( currentState.currentPosition == aim.position )
				break;

			Point nextPosition;
			for ( const Point &act: possibleAction ) { // Get next state
				nextPosition.setX( currentState.currentPosition.getX() + act.getX()); nextPosition.setY(currentState.currentPosition.getY() + act.getY());
				if( (explored.find( nextPosition ) == explored.end()) &&
					boxView[nextPosition.getY()][nextPosition.getX()] < 3 &&
					boxView[currentState.currentPosition.getY() - act.getY()][currentState.currentPosition.getX() - act.getX()]  != 3 ) {
						explored[nextPosition] = currentState.currentPosition;
						state nextState( manhattanDistance(it->first,nextPosition) + euclideanDistance(nextPosition, aim.position), nextPosition );
						frontier.push( nextState );
					}
			}
		}

		Point current = currentState.currentPosition;
		Point origin = it->first;
		barrier = 0;
		while ( current != origin  ) {
			if( boxView[current.getY()][current.getX()] == 2 )
				barrier++;
			current = explored[ current ];
		}
		it->second.barrier = barrier;
	}

	for ( it = boxes.begin(); it != boxes.end(); it++ )
		boxView[it->first.getY()][it->first.getX()] = 0;
}
