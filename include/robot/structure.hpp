
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
		virtual void Reset() = 0;
		std::string name;
		
		void Init();
		bool isInitialized = false;
	protected:
		virtual void Create() = 0;
};

#endif
