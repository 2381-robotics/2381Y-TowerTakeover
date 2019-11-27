
#ifndef STRUCTURE_H_
#define STRUCTURE_H_

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
