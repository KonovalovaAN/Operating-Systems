// Приведенный ниже блок ifdef — это стандартный метод создания макросов, упрощающий процедуру
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа BDLL_EXPORTS
// Символ, определенный в командной строке. Этот символ не должен быть определен в каком-либо проекте,
// использующем данную DLL. Благодаря этому любой другой проект, исходные файлы которого включают данный файл, видит
// функции BDLL_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.
#ifdef BDLL_EXPORTS
#define BDLL_API __declspec(dllexport)
#else
#define BDLL_API __declspec(dllimport)
#endif

// Этот класс экспортирован из библиотеки DLL
class BDLL_API CBDLL {
public:
	CBDLL(void);
	// TODO: добавьте сюда свои методы.
};

extern BDLL_API int nBDLL;

extern "C" BDLL_API int nBDLL;
extern "C" BDLL_API int fnBDLL(void);
extern BDLL_API HMODULE hModuleBDLL;
BDLL_API int fnBDLL(void);
