#ifndef CAMERA_H
#define CAMERA_H

#include "vector_util.h"
#include <SFML/System.hpp>

class Camera {
public:
    Camera(const sf::Vector2f& windowSize, const sf::Vector2f& viewCenterWorld,
           float worldUnitsPerPixel = 1.f)
        : mWindowSize{windowSize},
          mViewCenterWorld{viewCenterWorld},
          mWorldUnitsPerPixel{worldUnitsPerPixel} {}

    // all of these:

    sf::Vector2f windowToWorld(const sf::Vector2f& pWindow) { 
        // pWindow is in window coords
        // camera moved by some amount in world coords
        // some amount -> view center in window coords, - what would've been view center at 0,0 in window coords
        sf::Vector2f viewCenterWindow = mViewCenterWorld / mWorldUnitsPerPixel;
        sf::Vector2f originViewCenter = mWindowSize * 0.5f;
        sf::Vector2f movementDiff = viewCenterWindow - originViewCenter;
        // (pWindow + some amount in window) * worldunits/px = pWindow in world
        sf::Vector2f result = (pWindow + movementDiff) * mWorldUnitsPerPixel;
        
        return result; 
    };

    sf::Vector2f worldToWindow(const sf::Vector2f& pWorld) { 
        sf::Vector2f viewCenterWindow = mViewCenterWorld / mWorldUnitsPerPixel;
        sf::Vector2f originViewCenter = mWindowSize * 0.5f;
        sf::Vector2f movementDiff = viewCenterWindow - originViewCenter;
        sf::Vector2f result = pWorld / mWorldUnitsPerPixel - movementDiff;

        return result; 
    };

    sf::Vector2f windowPercentToWorld(const sf::Vector2f& pWindowPercent) { 
        sf::Vector2f windowPoint = pWindowPercent * mWindowSize;
    
        return windowToWorld(windowPoint); 
    };

    sf::Vector2f worldToWindowPercent(const sf::Vector2f& pWorld) { 
        sf::Vector2f pointInWindow = worldToWindow(pWorld);

        return pointInWindow / mWindowSize;
    };

    void translateWorld(const sf::Vector2f& deltaWorld) { mViewCenterWorld += deltaWorld; };

    void translateWindow(const sf::Vector2f& deltaScreen) { mViewCenterWorld += (deltaScreen * mWorldUnitsPerPixel); };

    void translateWindowPercent(const sf::Vector2f& deltaScreenPercent) {
        mViewCenterWorld += (deltaScreenPercent * mWindowSize * mWorldUnitsPerPixel);
    };

    void setCamera(const sf::Vector2f& pWorld, const sf::Vector2f& pWindow) {
        sf::Vector2f differenceWorld = (pWindow - (mWindowSize * 0.5f)) * mWorldUnitsPerPixel;
        
        mViewCenterWorld = pWorld - differenceWorld;
    };

    void setCameraPercent(const sf::Vector2f& pWorld, const sf::Vector2f& pWindowPercent) {
        sf::Vector2f windowPt = pWindowPercent * mWindowSize;
        setCamera(pWorld, windowPt);
    };

    void setCameraCenter(const sf::Vector2f& pWorld) { mViewCenterWorld = pWorld; };

    sf::Vector2f getCameraCenter() const { return mViewCenterWorld; };

private:
    sf::Vector2f mWindowSize;       // size of the window in screen pixels (i.e. window coords)
    sf::Vector2f mViewCenterWorld;  // world coords point corresponding to the center of our window.
    float mWorldUnitsPerPixel;      // distance in world coords covered by one pixel
};

#endif  // CAMERA_H
