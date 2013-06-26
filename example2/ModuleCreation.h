/// Author: Przemysław Bągard

#ifndef MODULECREATION_H
#define MODULECREATION_H

class Module;

/// Class responsible for creating the module.
class ModuleCreation
{
public:
    ModuleCreation();

    static void create(Module* m);
};

#endif // MODULECREATION_H
