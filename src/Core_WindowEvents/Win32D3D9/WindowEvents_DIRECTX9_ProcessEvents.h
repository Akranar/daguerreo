#ifndef _WINDOW_EVENTS_DIRECTX10_PROCESSEVENTS_H_
#define _WINDOW_EVENTS_DIRECTX10_PROCESSEVENTS_H_




//Static Message Processing
int WindowEvents::ProcessEvents()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT) 
		{
			return GetWindowCount();
		}
	}
	return GetWindowCount();
}

//Windows Message Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowEvents * targeted_window;
	switch (message) 
	{
		case WM_ACTIVATE:
			switch(wParam)
			{
				case WA_INACTIVE: 
					{
						//std::cout << "INACTIVE" << std::endl;
						InputEvent input_event;
						input_event.SetType(InputEvent::WINDOW_ACTIVATION);
						input_event.data.window_activation.SetActive(false);
						PushInputEvent(input_event);

						active_window = 0;
					}
					break;
				case WA_CLICKACTIVE:
					{
						//std::cout << "CLICKACTIVE" << std::endl;
						WindowEvents * matching_window = active_data->FindWindowEvents(hWnd);
						if (matching_window)
						{
							matching_window->MakeActive();

							InputEvent input_event;
							input_event.SetType(InputEvent::WINDOW_ACTIVATION);
							input_event.data.window_activation.SetActive(true);
							PushInputEvent(input_event);
							active_data->SetMouseRequiresUpdate(true);
						}
						PostMessage(hWnd, WM_INPUT, 0, (LPARAM) &Rid[0]);

						
					}
					break;
				case WA_ACTIVE:
					{
						//std::cout << "ACTIVE" << std::endl;
						WindowEvents * matching_window = active_data->FindWindowEvents(hWnd);
						if (matching_window)
						{
							matching_window->MakeActive();

							InputEvent input_event;
							input_event.SetType(InputEvent::WINDOW_ACTIVATION);
							input_event.data.window_activation.SetActive(true);
							PushInputEvent(input_event);
							active_data->SetMouseRequiresUpdate(true);
						}
					}
					break;
				default:
					break;
			}
			break;
		case WM_CAPTURECHANGED:
			{
				//std::cout << "CAPTURECHANGED" << std::endl;
				ReleaseCapture();
			}
			break;
		case WM_SIZE:
			{
				targeted_window = active_data->FindWindowEvents(hWnd);
				if (targeted_window)
				{
					InputEvent resize_event;
					resize_event.SetType(InputEvent::RESIZE);
					resize_event.data.resize.SetWidth(LOWORD(lParam));
					resize_event.data.resize.SetHeight(HIWORD(lParam));
					targeted_window->GetInputManager().PushEvent(resize_event);
				}
			}
			break;
		case WM_CLOSE:
			targeted_window = active_data->FindWindowEvents(hWnd);
			if (targeted_window)
			{
				targeted_window->Close();
			}
			break;
		case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			break;
	}

	if (!active_window) return DefWindowProc(hWnd, message, wParam, lParam);

	switch (message)
	{
		case WM_INPUT: 
			{
				if (wParam != RIM_INPUT) break;

				const MouseState & mouse_state = active_window->GetInputManager().GetMouseState();

				UINT dwSize = 40;
				static BYTE lpb[40];
				GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
				RAWINPUT* raw = (RAWINPUT*)lpb;

				if (raw->header.dwType == RIM_TYPEMOUSE) 
				{
					int x_relative = 0;
					int y_relative = 0;
					POINT cursor;
					GetCursorPos(&cursor);

					bool mouse_in_client_area = DefWindowProc(active_data->GetWindowHandle(), WM_NCHITTEST, 0, MAKELPARAM(cursor.x, cursor.y)) == HTCLIENT;

					RECT window_rect;
					GetWindowRect(hWnd,&window_rect);
					ScreenToClient(hWnd,&cursor);

					InputEvent mouse_event;
					mouse_event.type = InputEvent::MOUSE_MOTION;
					
					x_relative = raw->data.mouse.lLastX;
					y_relative = raw->data.mouse.lLastY;

					if (x_relative != 0 || y_relative != 0 || active_data->MouseRequiresUpdate())
					{
						mouse_event.SetType(InputEvent::MOUSE_MOTION);

						mouse_event.data.mouse_motion.SetX((short) cursor.x);
						mouse_event.data.mouse_motion.SetY((short) cursor.y);
						mouse_event.data.mouse_motion.SetRelX(x_relative);
						mouse_event.data.mouse_motion.SetRelY(y_relative);
						PushInputEvent(mouse_event);
						active_data->SetMouseRequiresUpdate(false);
					}

					USHORT usButtonData = raw->data.mouse.usButtonData;
					USHORT usButtonFlags = raw->data.mouse.usButtonFlags;
					if (usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN && mouse_in_client_area)
					{
						mouse_event.SetType(InputEvent::MOUSE_BUTTON);
						mouse_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_LEFT);
						mouse_event.data.mouse_button.SetState(MouseButtonEvent::PRESSED);
						PushInputEvent(mouse_event);
					}
					else if (usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP && mouse_state.GetLeft())
					{
						mouse_event.SetType(InputEvent::MOUSE_BUTTON);
						mouse_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_LEFT);
						mouse_event.data.mouse_button.SetState(MouseButtonEvent::RELEASED);
						PushInputEvent(mouse_event);
					}
					if (usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN && mouse_in_client_area)
					{
						mouse_event.SetType(InputEvent::MOUSE_BUTTON);
						mouse_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_MIDDLE);
						mouse_event.data.mouse_button.SetState(MouseButtonEvent::PRESSED);
						PushInputEvent(mouse_event);
					}
					else if (usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP && mouse_state.GetMiddle())
					{
						mouse_event.SetType(InputEvent::MOUSE_BUTTON);
						mouse_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_MIDDLE);
						mouse_event.data.mouse_button.SetState(MouseButtonEvent::RELEASED);
						PushInputEvent(mouse_event);
					}
					if (usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN && mouse_in_client_area)
					{
						mouse_event.SetType(InputEvent::MOUSE_BUTTON);
						mouse_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_RIGHT);
						mouse_event.data.mouse_button.SetState(MouseButtonEvent::PRESSED);
						PushInputEvent(mouse_event);
					}
					else if (usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP && mouse_state.GetRight())
					{
						mouse_event.SetType(InputEvent::MOUSE_BUTTON);
						mouse_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_RIGHT);
						mouse_event.data.mouse_button.SetState(MouseButtonEvent::RELEASED);
						PushInputEvent(mouse_event);
					}
					if (usButtonFlags & RI_MOUSE_BUTTON_4_DOWN && mouse_in_client_area)
					{
						mouse_event.SetType(InputEvent::MOUSE_BUTTON);
						mouse_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_4);
						mouse_event.data.mouse_button.SetState(MouseButtonEvent::PRESSED);
						PushInputEvent(mouse_event);
					}
					else if (usButtonFlags & RI_MOUSE_BUTTON_4_UP && mouse_state.GetFourth())
					{
						mouse_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_4);
						mouse_event.data.mouse_button.SetState(MouseButtonEvent::RELEASED);
						PushInputEvent(mouse_event);
					}
					if (usButtonFlags & RI_MOUSE_BUTTON_5_DOWN && mouse_in_client_area)
					{
						mouse_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_5);
						mouse_event.data.mouse_button.SetState(MouseButtonEvent::PRESSED);
						PushInputEvent(mouse_event);
					}
					else if (usButtonFlags & RI_MOUSE_BUTTON_5_UP && mouse_state.GetFifth())
					{
						mouse_event.data.mouse_button.SetButton(MouseButtonEvent::BUTTON_5);
						mouse_event.data.mouse_button.SetState(MouseButtonEvent::RELEASED);
						PushInputEvent(mouse_event);
					}
					if (usButtonFlags & RI_MOUSE_WHEEL)
					{
						signed short magnitude = (signed short) usButtonData;
						if (magnitude > 0)
						{
							mouse_event.SetType(InputEvent::MOUSE_WHEEL);
							mouse_event.data.mouse_wheel.SetDirection(MouseWheelEvent::UP);
							PushInputEvent(mouse_event);
						}
						else
						{
							mouse_event.SetType(InputEvent::MOUSE_WHEEL);
							mouse_event.data.mouse_wheel.SetDirection(MouseWheelEvent::DOWN);
							PushInputEvent(mouse_event);
						}
					}	
				}
				else if (raw->header.dwType == RIM_TYPEKEYBOARD)
				{
					USHORT virtual_key_code = raw->data.keyboard.VKey;
					const KeyState & key_state = active_window->GetInputManager().GetKeyState();

					InputEvent input_event;
					input_event.SetType(InputEvent::KEYBOARD);

					switch(raw->data.keyboard.Message)
					{
						case WM_KEYDOWN:
						{
							
							switch(virtual_key_code)
							{
								case VK_SHIFT:
								{
									
									bool left = HIBYTE(GetAsyncKeyState(VK_LSHIFT)) != 0;
									bool right = HIBYTE(GetAsyncKeyState(VK_RSHIFT)) != 0;
									
									if (left)
									{
										if (!key_state.GetState(KeyID::KEY_LEFT_SHIFT))
										{
											input_event.data.key.SetType(KeyEvent::KEY_DOWN);
											input_event.data.key.SetKey(KeyID::KEY_LEFT_SHIFT);
											PushInputEvent(input_event);
											//std::cout << "Left Shift Down: " << std::endl;
										}
										else if (active_window->AutoRepeat())
										{
											input_event.data.key.SetType(KeyEvent::KEY_REPEAT);
											input_event.data.key.SetKey(KeyID::KEY_LEFT_SHIFT);
											PushInputEvent(input_event);
											//std::cout << "Left Shift Repeat: " << std::endl;
										}
									}
									if (right)
									{
										if (!key_state.GetState(KeyID::KEY_RIGHT_SHIFT))
										{
											input_event.data.key.SetType(KeyEvent::KEY_DOWN);
											input_event.data.key.SetKey(KeyID::KEY_RIGHT_SHIFT);
											PushInputEvent(input_event);
											//std::cout << "Right Shift Down: " << std::endl;
										}
										else if (active_window->AutoRepeat())
										{
											input_event.data.key.SetType(KeyEvent::KEY_REPEAT);
											input_event.data.key.SetKey(KeyID::KEY_RIGHT_SHIFT);
											PushInputEvent(input_event);
											//std::cout << "Right Shift Repeat: " << std::endl;
										}
									}
								}
								break;
								case VK_CONTROL:
								{
									bool left = HIBYTE(GetAsyncKeyState(VK_LCONTROL)) != 0;
									bool right = HIBYTE(GetAsyncKeyState(VK_RCONTROL)) != 0;

									if (left)
									{
										if (!key_state.GetState(KeyID::KEY_LEFT_CTRL))
										{
											input_event.data.key.SetType(KeyEvent::KEY_DOWN);
											input_event.data.key.SetKey(KeyID::KEY_LEFT_CTRL);
											PushInputEvent(input_event);
											//std::cout << "Left Control Down: " << std::endl;
										}
										else if (active_window->AutoRepeat())
										{
											input_event.data.key.SetType(KeyEvent::KEY_REPEAT);
											input_event.data.key.SetKey(KeyID::KEY_LEFT_CTRL);
											PushInputEvent(input_event);
											//std::cout << "Left Control Repeat: " << std::endl;
										}
									}
									if (right)
									{
										if (!key_state.GetState(KeyID::KEY_RIGHT_CTRL))
										{
											input_event.data.key.SetType(KeyEvent::KEY_DOWN);
											input_event.data.key.SetKey(KeyID::KEY_RIGHT_CTRL);
											PushInputEvent(input_event);
											//std::cout << "Right Control Down: " << std::endl;
										}
										else if (active_window->AutoRepeat())
										{
											input_event.data.key.SetType(KeyEvent::KEY_REPEAT);
											input_event.data.key.SetKey(KeyID::KEY_RIGHT_CTRL);
											PushInputEvent(input_event);
											//std::cout << "Right Control Repeat: " << std::endl;
										}
									}
								}
								break;
								case VK_MENU:
								{
									bool left = HIBYTE(GetAsyncKeyState(VK_LMENU)) != 0;
									bool right = HIBYTE(GetAsyncKeyState(VK_RMENU)) != 0;

									if (left)
									{
										if (!key_state.GetState(KeyID::KEY_LEFT_ALT))
										{
											input_event.data.key.SetType(KeyEvent::KEY_DOWN);
											input_event.data.key.SetKey(KeyID::KEY_LEFT_ALT);
											PushInputEvent(input_event);
											//std::cout << "Left Alt Down: " << std::endl;
										}
										else if (active_window->AutoRepeat())
										{
											input_event.data.key.SetType(KeyEvent::KEY_REPEAT);
											input_event.data.key.SetKey(KeyID::KEY_LEFT_ALT);
											PushInputEvent(input_event);
											//std::cout << "Left Alt Repeat: " << std::endl;
										}
									}
									if (right)
									{
										if (!key_state.GetState(KeyID::KEY_RIGHT_ALT))
										{
											input_event.data.key.SetType(KeyEvent::KEY_DOWN);
											input_event.data.key.SetKey(KeyID::KEY_RIGHT_ALT);
											PushInputEvent(input_event);
											//std::cout << "Right Alt Down: " << std::endl;
										}
										else if (active_window->AutoRepeat())
										{
											input_event.data.key.SetType(KeyEvent::KEY_REPEAT);
											input_event.data.key.SetKey(KeyID::KEY_RIGHT_ALT);
											PushInputEvent(input_event);
											//std::cout << "Right Alt Repeat: " << std::endl;
										}
									}
								}
								break;
								default:
								{
									
									KeyID::Key key_id = KeyMap[virtual_key_code];
									if (key_state.GetState(key_id))
									{
										if (active_window->AutoRepeat())
										{
											input_event.data.key.SetType(KeyEvent::KEY_REPEAT);
										}
									}
									else
									{
										//std::cout << virtual_key_code << ": down" << std::endl;
										input_event.data.key.SetType(KeyEvent::KEY_DOWN);
									}
									input_event.data.key.SetKey(key_id);
									PushInputEvent(input_event);
								}
								break;
							}
						}
						break;
						case WM_KEYUP:
						{
							switch(virtual_key_code)
							{
								case VK_SHIFT:
								{
									bool left = HIBYTE(GetAsyncKeyState(VK_LSHIFT)) != 0;
									bool right = HIBYTE(GetAsyncKeyState(VK_RSHIFT)) != 0;

									input_event.data.key.SetType(KeyEvent::KEY_UP);
									if (!left && key_state.GetState(KeyID::KEY_LEFT_SHIFT))
									{
										input_event.data.key.SetKey(KeyID::KEY_LEFT_SHIFT);
										PushInputEvent(input_event);
										//std::cout << "Left Shift Up: " << std::endl;
									}
									if (!right && key_state.GetState(KeyID::KEY_RIGHT_SHIFT))
									{
										input_event.data.key.SetKey(KeyID::KEY_RIGHT_SHIFT);
										PushInputEvent(input_event);
										//std::cout << "Right Shift Up: " << std::endl;
									}
								}
								break;
								case VK_CONTROL:
								{
									bool left = HIBYTE(GetAsyncKeyState(VK_LCONTROL)) != 0;
									bool right = HIBYTE(GetAsyncKeyState(VK_RCONTROL)) != 0;

									input_event.data.key.SetType(KeyEvent::KEY_UP);
									if (!left && key_state.GetState(KeyID::KEY_LEFT_CTRL))
									{
										input_event.data.key.SetKey(KeyID::KEY_LEFT_CTRL);
										PushInputEvent(input_event);
										//std::cout << "Left Control Up: " << std::endl;
									}
									if (!right && key_state.GetState(KeyID::KEY_RIGHT_CTRL))
									{
										input_event.data.key.SetKey(KeyID::KEY_RIGHT_CTRL);
										PushInputEvent(input_event);
										//std::cout << "Right Control Up: " << std::endl;
									}
								}
								break;
								case VK_MENU:
								{
									bool left = HIBYTE(GetAsyncKeyState(VK_LMENU)) != 0;
									bool right = HIBYTE(GetAsyncKeyState(VK_RMENU)) != 0;

									input_event.data.key.SetType(KeyEvent::KEY_UP);
									if (!left && key_state.GetState(KeyID::KEY_LEFT_ALT))
									{
										input_event.data.key.SetKey(KeyID::KEY_LEFT_ALT);
										PushInputEvent(input_event);
										//std::cout << "Left Alt Up: " << std::endl;
									}
									if (!right && key_state.GetState(KeyID::KEY_RIGHT_ALT))
									{
										input_event.data.key.SetKey(KeyID::KEY_RIGHT_ALT);
										PushInputEvent(input_event);
										//std::cout << "Right Alt Up: " << std::endl;
									}
								}
								break;
								default:
								{
									KeyID::Key key_id = KeyMap[virtual_key_code];
									input_event.data.key.SetType(KeyEvent::KEY_UP);
									input_event.data.key.SetKey(key_id);
									PushInputEvent(input_event);
									//std::cout << virtual_key_code << ": up" << std::endl;
								}
								break;
							}
						}	
						break;
					}
				}
			}
			break;	
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

#endif