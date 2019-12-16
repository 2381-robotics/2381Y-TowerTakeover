
#ifndef STRUCTURE_HPP_
#define STRUCTURE_HPP_
#include <map>
#include <vector>
class Structure {
	public:
		/**
   		* Constructor
   * Abstract Parent Class, is inherited by structural objects
   */
		Structure(){};
		virtual void create() = 0;
		virtual void Reset() = 0;

		std::string name;
		// virtual void Debug() = 0;


	protected:
};

#endif
