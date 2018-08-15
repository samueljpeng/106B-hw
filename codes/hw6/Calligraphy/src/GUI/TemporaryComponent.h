#ifndef TemporaryComponent_Included
#define TemporaryComponent_Included

#include "gwindow.h"
#include "gobjects.h"
#include <string>

/* A type representing a graphics object with a short lifetime. The object
 * is created and installed when the Temporary is created, and it is
 * removed and deallocated when the Temporary is destroyed.
 */
template <typename Component> class Temporary {
public:
    /* Constructor installs the new component. */
    Temporary(Component* component, GWindow& window, const std::string& location);

    /* Destructor removes it. */
    ~Temporary();

    /* Access underlying component. */
    Component* get() const;
    Component& operator*  () const;
    Component* operator-> () const;

private:
    /* Copying is not allowed. */
    Temporary(const Temporary &) = delete;
    void operator= (Temporary) = delete;

    /* Underlying component, window, and location. */
    Component* const component;
    GWindow& window;
    const std::string location;
};

/* * * * * Implementation Below This Point * * * * */
template <typename Component>
Temporary<Component>::Temporary(Component* component,
                                GWindow& window,
                                const std::string& location)
    : component(component), window(window), location(location) {
    window.addToRegion(component, location);
}

/* Destructor removes and frees the component. */
template <typename Component>
Temporary<Component>::~Temporary() {
    window.removeFromRegion(component, location);
    delete component;
}

/* Accessor just hands back the underlying component. */
template <typename Component>
Component* Temporary<Component>::get() const {
    return component;
}

/* Star and arrow operators. */
template <typename Component>
Component& Temporary<Component>::operator *() const {
    return *component;
}
template <typename Component>
Component* Temporary<Component>::operator->() const {
    return &**this;
}

#endif
