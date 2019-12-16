
#ifndef STRUCTURE_HPP_
#define STRUCTURE_HPP_

class Structure {
	public:
		/**
   		* Constructor
   * Abstract Parent Class, is inherited by structural objects
   */
		Structure(){};
		virtual void create() = 0;

	protected:
};

#endif
