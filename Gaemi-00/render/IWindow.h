#pragma once
#ifndef RENDER_IWINDOW_H
#define RENDER_IWINDOW_H

#include "../Defines.h"
#include "../GameTime.h"

#include <memory>
#include <SDL.h>
#include "../math/RectangleInt.h"

constexpr f64 DURATION_BETWEEN_FPS_DISPLAY = 0.25;

namespace engine::render {

	class IWindow
	{
	public:
        virtual ~IWindow() = default;

		/// <summary>
		/// Initialize window with correct size and position
		/// </summary>
		/// <param name="x">Window horizontal position</param>
		/// <param name="y">Windows vertical position</param>
		/// <param name="width">Window's width</param>
		/// <param name="height">Window's hight</param>
		/// <param name="isFullscreen">True is window is fullscreen</param>
		/// <returns>True when properly initialized</returns>
        virtual bool init(i32 x, i32 y, i32 width, i32 height, bool isFullscreen) = 0;

		/// <summary>
		/// Clean up window
		/// </summary>
        virtual void close() = 0;

		/// <summary>
		/// Write FPS number on window
		/// </summary>
		/// <param name="time">Game time</param>
        virtual void updateFPSCounter(const GameTime& time) = 0;

        /// Return window size and position
        /// \return Bounds rectangle
        virtual const gmath::RectangleInt& getBounds() const = 0;
	};

}

#endif // RENDER_IWINDOW_H
