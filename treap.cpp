// CMSC 341 - Fall 2019 - Project 3
// File: treap.cpp

using namespace std ;

#include "treap.h"

Treap::Treap() {
  _nptr = nullptr ;
}

Treap::Treap(const Treap& other) {

	_nptr = nullptr;
	preorder(other._nptr);
	updateHeight();
}

Treap::~Treap() {
  
	//use postorder to delete each node
	postorder();
}

const Treap& Treap::operator=(const Treap& rhs) {
  //
  // Implement assignment operator
	preorder(rhs._nptr);
	updateHeight();
	return *this;
}

// Return -1 if the treap is empty; otherwise, return value in _height.
int Treap::height() const {
  return _nptr ? _nptr->_height : -1 ;
}

// Return the priority of the root node of a Treap.
// Note: numeric_limits<priority_t>::min() returns the minimum value
// of the priority type.  We will only use int priorities, so this is
// the minimum integer value.
priority_t Treap::priority() const {
  return _nptr ? _nptr->_pri : numeric_limits<priority_t>::min() ;
}

const data_t* Treap::find(const data_t& x) {

	data_t* point = nullptr;
	TreapNode* trav = _nptr;

	if (empty())
	{
		return point;
	}

	while (trav->_data != x)
	{
		if (trav->_data > x)
		{
			trav = trav->_left._nptr;
		}
		else
		{
			trav = trav->_right._nptr;
		}
	}

	//root
	if (trav->_data == x)
	{
		point = &trav->_data;
		return point;
	}
	return point;
}

// Inorder traversal.  Recommend you NOT change this!!!
void Treap::inorder() {

  if ( empty() ) return ;

  cout << "(" ;
  _nptr->_left.inorder() ;

  cout << _nptr->_data << ":" << priority() << ":" << height() ;

  _nptr->_right.inorder() ;
  cout << ")" ;

  return  ;
}

//preorder used to copy tree
void Treap::preorder(TreapNode* x) {

	if (empty() && (x->_left._nptr == nullptr &&
		x->_right._nptr == nullptr)) {
		if (x->_left._nptr == nullptr &&
			x->_right._nptr == nullptr)
		{
			insert(x->_data, x->_pri);
		}
		return;
	}
	else
	{
		//root
		insert(x->_data, x->_pri);

		//left
		if (x->_left._nptr != nullptr)
		{
			_nptr->_left.preorder(x->_left._nptr);
		}

		//right
		if (x->_right._nptr != nullptr)
		{
			_nptr->_right.preorder(x->_right._nptr);
		}

		return  ;
	}
}

//postorder to delete each node
void Treap::postorder()
{
	if (empty() || (_nptr->_left._nptr == nullptr && 
		_nptr->_right._nptr == nullptr)) {
		remove(_nptr->_data);
		_nptr = nullptr;
		return;
	}
	else
	{
		//left

		_nptr->_left.postorder();

		//right
		_nptr->_right.postorder();

		//root
		this->postorder();
		return;
	}
}

// Used for debugging and grading.  Interprets the characters in
// position[] as directions in the tree.  E.g., "RLR" means, "starting
// from root, go right, then left, then right."  If position[] is an
// empty string, then it just locats the value a the root.
// See project description for details.
bool Treap::locate (const char position[], data_t& x, priority_t& p, int& h, int offset) {

  if (_nptr == nullptr) return false ;

  if (position[offset] == '\0') {
    x = _nptr->_data ;
    p = _nptr->_pri ;
    h = _nptr->_height ;
    return true ;
  }

  if (position[offset] == 'L') {
    return _nptr->_left.locate(position, x, p, h, offset+1) ;
  }

  if (position[offset] == 'R') {
    return _nptr->_right.locate(position, x, p, h, offset+1) ;
  }

  cerr << "Bad position character!\n" ;
  exit(17) ;

  return false ; 
}


// Basic BST insertion.  Does not allow duplicate values.
//
// Complete this function to do Treap insertion.
//
void Treap::insert(const data_t& x, const priority_t& p) {
  if ( empty() ) {
    _nptr = new TreapNode(x, p) ;
  }

  if (_nptr->_data < x) { 
    _nptr->_right.insert(x, p) ;
  } else if (x < _nptr->_data ) {
    _nptr->_left.insert(x, p) ;
  }
 
  heapOps(_nptr);
  updateHeight();
}

void Treap::heapOps(TreapNode* GP)
{
	//case where depth of tree is less than 3
	if ((GP->_height < 2 && GP->_height != 0) || 
		(GP->_right._nptr != nullptr && GP->_pri < GP->_right._nptr->_pri) ||
		(GP->_left._nptr != nullptr && GP->_pri < GP->_left._nptr->_pri))
	{
		data_t tempData = GP->_data;
		int tempPri = GP->_pri;
		//check left
		if (GP->_left._nptr != nullptr)
		{
			if (GP->_pri < GP->_left._nptr->_pri)
			{
				TreapNode *temp = GP->_left._nptr;
				TreapNode *tempT = GP->_right._nptr;

				GP->_data = temp->_data;
				GP->_pri = temp->_pri;
				temp->_data = tempData;
				temp->_pri = tempPri;
				GP->_left._nptr = temp->_left._nptr;
				temp->_left._nptr = temp->_right._nptr;
				temp->_right._nptr = GP->_right._nptr;
				GP->_right._nptr = temp;
			}
		}
		//check right
		if (GP->_right._nptr != nullptr)
		{
			if (GP->_pri < GP->_right._nptr->_pri)
			{
				TreapNode *temp = GP->_right._nptr;
				TreapNode *tempT = GP->_left._nptr;

				GP->_data = temp->_data;
				GP->_pri = temp->_pri;
				temp->_data = tempData;
				temp->_pri = tempPri;
				GP->_right._nptr = temp->_right._nptr;
				temp->_right._nptr = temp->_left._nptr;
				temp->_left._nptr = GP->_left._nptr;
				GP->_left._nptr = temp;
			}
		}
	}
	else if (GP->_height != 0)
	{
		bool leftSide = true;
		//LL and LR
		if (GP->_left._nptr != nullptr &&
			GP->_left._nptr->_left._nptr != nullptr &&
			GP->_left._nptr->_right._nptr != nullptr)
		{
			if (GP->_left._nptr->_pri < GP->_left._nptr->_left._nptr->_pri ||
				GP->_left._nptr->_pri < GP->_left._nptr->_right._nptr->_pri)
			{
				//LR
				if (GP->_left._nptr->_pri < GP->_left._nptr->_right._nptr->_pri)
				{
					leftSide = false;
				}

				TreapNode *tempPar = GP->_left._nptr;
				TreapNode *tempChi = tempPar->_left._nptr;
				TreapNode *tempGC = tempChi->_left._nptr;
				data_t tempData = tempPar->_data;
				int tempPri = tempPar->_pri;

				if (!leftSide)
				{
					tempChi = tempPar->_right._nptr;
					tempGC = tempChi->_right._nptr;
				}

				tempPar->_data = tempChi->_data;
				tempPar->_pri = tempChi->_pri;
				tempChi->_data = tempData;
				tempChi->_pri = tempPri;

				if (leftSide)
				{
					tempChi->_right._nptr = tempPar->_right._nptr;
					tempPar->_right._nptr = tempChi;
					tempPar->_left._nptr = tempChi->_left._nptr;
					tempChi->_right._nptr = tempGC;
				}
				else
				{
					tempChi->_left._nptr = tempPar->_left._nptr;
					tempPar->_left._nptr = tempChi;
					tempPar->_right._nptr = tempChi->_right._nptr;
					tempChi->_left._nptr = tempGC;
				}

				GP->_left._nptr = tempPar;

				//after rotating, check again before moving back up the tree
				if (GP->_left._nptr->_pri > GP->_pri)
				{
					heapOps(GP);
				}
			}
		}
		//RR and RL
		else if (GP->_right._nptr != nullptr &&
			GP->_right._nptr->_right._nptr != nullptr &&
			GP->_right._nptr->_left._nptr != nullptr)
		{
			if (GP->_right._nptr->_pri < GP->_right._nptr->_right._nptr->_pri ||
				GP->_right._nptr->_pri < GP->_right._nptr->_left._nptr->_pri)
			{
				//RR
				if (GP->_right._nptr->_pri < GP->_right._nptr->_right._nptr->_pri)
				{
					leftSide = false;
				}

				TreapNode *tempPar = GP->_right._nptr;
				TreapNode *tempChi = tempPar->_right._nptr;
				TreapNode *tempGC = tempChi->_left._nptr;
				data_t tempData = tempPar->_data;
				int tempPri = tempPar->_pri;

				if (leftSide)
				{
					tempChi = tempPar->_left._nptr;
					tempGC = tempChi->_right._nptr;
				}

				tempPar->_data = tempChi->_data;
				tempPar->_pri = tempChi->_pri;
				tempChi->_data = tempData;
				tempChi->_pri = tempPri;

				if (!leftSide)
				{
					tempChi->_left._nptr = tempPar->_left._nptr;
					tempPar->_left._nptr = tempChi;
					tempPar->_right._nptr = tempChi->_right._nptr;
					tempChi->_left._nptr = tempGC;
				}
				else
				{
					tempChi->_right._nptr = tempPar->_right._nptr;
					tempPar->_right._nptr = tempChi;
					tempPar->_left._nptr = tempChi->_left._nptr;
					tempChi->_right._nptr = tempGC;
				}

				//after rotating, check again before moving back up the tree
				GP->_right._nptr = tempPar;
				if (GP->_right._nptr->_pri > GP->_pri)
				{
					heapOps(GP);
				}
			}
		}
		else
		{
			//if (GP->_height < 2)
			//{
			//	return;
			//}
			if (GP->_right._nptr == nullptr ||
				GP->_right._nptr->_right._nptr == nullptr ||
				GP->_right._nptr->_left._nptr == nullptr ||
				GP->_left._nptr == nullptr ||
				GP->_left._nptr->_right._nptr == nullptr ||
				GP->_left._nptr->_left._nptr == nullptr)
			{
				return;
			}
			else
			{
				heapOps(GP->_left._nptr);
				heapOps(GP->_right._nptr);
			}
		}
	}
}

void Treap::updateHeight()
{
	if (_nptr != nullptr && _nptr->_left._nptr != nullptr)
	{
		_nptr->_left.updateHeight();
	}
	if (_nptr != nullptr && _nptr->_right._nptr != nullptr)
	{
		_nptr->_right.updateHeight();
	}
	// Update height. Maybe this should be a helper function??
	int leftHeight = _nptr->_left.height();
	int rightHeight = _nptr->_right.height();
	_nptr->_height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

bool Treap::remove(const data_t& x) {

	bool found = false;
	TreapNode *trav = _nptr;
	TreapNode* curr = trav;

	const data_t* t = find(x);

	//if find returns a nullptr, the data dowsn't exist
	//and this func will return false
	if (t != nullptr)
	{
		found = true;

			if ((curr->_left._nptr != nullptr && curr->_left._nptr->_data != x) &&
				(curr->_right._nptr != nullptr && curr->_right._nptr->_data != x))
			{
				while ((curr->_left._nptr != nullptr && curr->_left._nptr->_data != x) ||
					(curr->_right._nptr != nullptr && curr->_right._nptr->_data != x))
				{
					if ((curr->_left._nptr != nullptr && curr->_left._nptr->_data == x) || 
						(curr->_right._nptr != nullptr && curr->_right._nptr->_data == x))
					{
						break;
					}
					if (curr->_left._nptr != nullptr && curr->_data > x)
					{
						curr = curr->_left._nptr;
					}
					else
					{
						curr = curr->_right._nptr;
					}
				}
			}

			TreapNode *del = revHeapOps(curr, x);
			propDel(curr, del);

			del->_left._nptr = nullptr;
			del->_right._nptr = nullptr;
			if (this->_nptr->_left._nptr == del)
			{
				this->_nptr->_left._nptr = nullptr;
			}
			else if (this->_nptr->_right._nptr == del)
			{
				this->_nptr->_right._nptr = nullptr;
			}
			updateHeight();
			delete del;
			del = nullptr;
	}
	return found;
}

void Treap::propDel(TreapNode* x, TreapNode* y)
{
	while ((x->_left._nptr != nullptr && x->_left._nptr->_data != y->_data) || 
		(x->_right._nptr != nullptr && x->_right._nptr->_data != y->_data))
	{
		if ((x->_left._nptr != nullptr && x->_left._nptr->_data == y->_data) ||
			(x->_right._nptr != nullptr && x->_right._nptr->_data == y->_data))
		{
			break;
		}
		if (x->_data > y->_data)
		{
			x = x->_left._nptr;
		}
		else
		{
			x = x->_right._nptr;
		}
	}
	if (x->_left._nptr == y)
	{
		x->_left._nptr = nullptr;
	}
	else if (x->_right._nptr == y)
	{
		x->_right._nptr = nullptr;
	}
}

TreapNode* Treap::revHeapOps(TreapNode* curr, data_t x)
{
	TreapNode *f = curr;
	if (curr->_left._nptr != nullptr && curr->_left._nptr->_data == x)
	{
		//shit down
		while ((curr->_left._nptr->_left._nptr != nullptr ||
			curr->_left._nptr->_right._nptr != nullptr) && curr->_left._nptr->_data == x)
		{
			TreapNode *tempPar = curr->_left._nptr;
			TreapNode *tempChi;
			if (tempPar->_left._nptr != nullptr)
			{
				tempChi = tempPar->_left._nptr;
				tempPar->_left._nptr = tempChi->_right._nptr;
				tempChi->_right._nptr = tempPar;
			}
			else
			{
				tempChi = tempPar->_right._nptr;
				tempPar->_right._nptr = tempChi->_left._nptr;
				tempChi->_left._nptr = tempPar;
			}
			curr->_left._nptr = tempChi;

			f = revHeapOps(curr->_left._nptr, x);
		}
		TreapNode *ret = nullptr;
		if (f->_data == x)
		{
			ret = f;
		}
		else
		{
			ret = f->_left._nptr;
		}
		updateHeight();
		return ret;
	}
	else if (curr->_right._nptr != nullptr && curr->_right._nptr->_data == x)
	{
		//shit down
		while ((curr->_right._nptr->_left._nptr != nullptr ||
			curr->_right._nptr->_right._nptr != nullptr) && curr->_right._nptr->_data == x)
		{
			TreapNode *tempPar = curr->_right._nptr;
			TreapNode *tempChi;
			if (tempPar->_left._nptr != nullptr)
			{
				tempChi = tempPar->_left._nptr;
				tempPar->_left._nptr = tempChi->_right._nptr;
				tempChi->_right._nptr = tempPar;
			}
			else
			{
				tempChi = tempPar->_right._nptr;
				tempPar->_right._nptr = tempChi->_left._nptr;
				tempChi->_left._nptr = tempPar;
			}
			curr->_right._nptr = tempChi;

			f = revHeapOps(curr->_right._nptr, x);
		}
		TreapNode *ret = nullptr;
		if (f->_data == x)
		{
			ret = f;
		}
		else
		{
			ret = f->_right._nptr;
		}		
		updateHeight();
		return ret;
	}
	else
	{
		TreapNode *ret = curr;
		updateHeight();
		return ret;
	}
}

// Treap::dump() just calls TreapNode::dump()
void Treap::dump() {
  if ( !empty() ) _nptr->dump() ;
}

ostream& operator<< (ostream& os, const Treap& T) {
  os << T._nptr ; 
  return os ;
}


TreapNode::TreapNode() : _height(0) {
  // Empty constructor body
}

TreapNode::TreapNode(const data_t& x, const priority_t& p) 
  : _data(x), _pri(p), _height(0) { 
  // Empty constructor body
}

TreapNode::~TreapNode() {
  // Empty destructor
  // Retained for debugging
}

// Dump the contents of a node
void TreapNode::dump() {
  cout << "===== TreapNode::dump()\n" ;
  cout << "      this = " << this << endl ; 
  cout << "      left = " << _left << endl ; 
  cout << "      right= " << _right << endl ; 
  cout << "      data = " << _data << endl ;
  cout << "      height = " << _height << endl ;
  _left.dump() ;
  _right.dump() ;
}

