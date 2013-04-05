#include <iostream>
#include <typeinfo>
#include <string>
#include <map>
namespace std{
	class Test {
		
		public:
			string getClassName();
			Test();
	};		   
	
	string Test::getClassName(){
		return "Test";
	}
	Test::Test(){}
	
	
	class EntityView {
		
		public:
			EntityView();
	};	

	EntityView::EntityView(){}
		
	class TestView : public EntityView {
		
		public:
			void printRight();
			TestView();
	};	
		  
	void TestView::printRight(){
		cout << "Success!!!!"<<endl;
	}
	
	TestView::TestView(){
		printRight();
	}

	class ViewFactory{
	
		public:
			ViewFactory();
	}
	template<typename T> EntityView * createInstance() { return new T; }
	
}
    
int main(){ 
	using namespace std;
	

	typedef std::map<std::string,EntityView*(*)()> map_type; 
	
	Test test = Test(); 
	string nombre = test.getClassName();
	
	map_type map;
	string viewName = nombre+"View";

	map[viewName] = &createInstance<TestView>;	
	map[viewName]();
	
	
	return 0;	
    
}
