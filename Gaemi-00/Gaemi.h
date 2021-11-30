// Le bloc ifdef suivant est la façon standard de créer des macros qui facilitent l'exportation
// à partir d'une DLL plus simple. Tous les fichiers contenus dans cette DLL sont compilés avec le symbole GAEMI00_EXPORTS
// défini sur la ligne de commande. Ce symbole ne doit pas être défini pour un projet
// qui utilise cette DLL. Ainsi, les autres projets dont les fichiers sources comprennent ce fichier considèrent les fonctions
// GAPI comme étant importées à partir d'une DLL, tandis que cette DLL considère les symboles
// définis avec cette macro comme étant exportés.
#ifdef GAEMI00_EXPORTS
#define GAPI __declspec(dllexport)
#else
#define GAPI __declspec(dllimport)
#endif

#include "Defines.h"

GAPI int display(const str& message);
