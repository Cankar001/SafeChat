/*****************************************************************
 * \file   SafeList.h
 * \brief
 *
 * \author Can Karka
 * \date   January 2021
 *
 * Copyright (C) 2021 Can Karka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see https://www.gnu.org/licenses/.
 *********************************************************************/

#pragma once

template<typename T>
class SafeList
	{
	private:
		struct Node
			{
			T Value;
			Node *Next;
			Node(T value) : Value(value), Next(nullptr) {}
			};

		void Append(Node *parent, T value)
			{
			if (parent->Next == nullptr)
				{
				parent->Next = new Node(value);
				++Length;
				}
			else
				{
				Append(parent->Next, value);
				}
			}

		Node *Root;
		uint32 Length;

	public:

		SafeList()
			{
			Length = 0;
			Root = nullptr;
			}

		~SafeList()
			{

			}

		void Append(T value)
			{
			if (Root == nullptr)
				{
				Root = new Node(value);
				++Length;
				}
			else
				{
				Append(Root, value);
				}
			}

		void Insert(T value)
			{
			Node *node = new Node(value);
			node->Next = Root;
			Root = node;
			++Length;
			}

		T GetFirst()
			{
			return Root->Value;
			}

		T GetLast()
			{
			Node *current = Root;
			if (current == nullptr)
				return nullptr;

			while (current->Next != nullptr)
				{
				current = current->Next;
				}

			return current->Value;
			}

		T Get(uint32 index)
			{
			if (index <= 0)
				return GetFirst();

			if (index >= Length)
				return GetLast();

			Node *current = Root;
			T ret = NULL;
			uint32 i = 0;

			if (current == nullptr)
				return nullptr;

			while (current != nullptr)
				{
				if (i == index)
					{
					ret = current->Value;
					break;
					}

				current = current->Next;
				i++;
				}

			return ret;
			}

		void RemoveFirst()
			{
			Node *tmp = Root;
			if (tmp == nullptr)
				return;

			Root = Root->Next;

			delete tmp;
			tmp = nullptr;
			--Length;
			}

		void RemoveLast()
			{
			if (Length == 1)
				{
				delete Root;
				Root = nullptr;
				Length = 0;
				return;
				}

			--Length;
			Node *current = Root;
			if (current == nullptr)
				return;

			while (current != nullptr)
				{
				if (current->Next->Next == nullptr)
					{
					delete current->Next;
					current->Next = nullptr;
					break;
					}

				current = current->Next;
				}
			}

		void Remove(uint32 length)
			{
			if (index == 0)
				{
				RemoveFirst();
				return;
				}

			if (Length == index)
				{
				RemoveLast();
				return;
				}

			Node *current = Root;
			if (current == nullptr)
				return;

			uint32 i = 0;
			--Length;

			while (current != nullptr)
				{
				if ((i + 1) == index)
					{
					Node *tmp = current->Next;
					current->Next = current->Next->Next;

					delete tmp;
					tmp = nullptr;

					break;
					}
				current = current->Next;
				}
			}

		void Clear()
			{
			if (Length > 0)
				{
				Node *current = Root;
				if (current == nullptr)
					return;

				while (current != nullptr)
					{
					Node *tmp = current->Next;
					delete current;
					current = tmp;
					}

				Root = nullptr;
				Length = 0;
				}
			}

		void RemoveAll()
			{
			Clear();
			delete Root;
			Root = nullptr;
			}

		bool IsEmpty()
			{
			return Length == 0;
			}

		uint32 Size()
			{
			return Length;
			}

		T *ToArray()
			{
			T *arr = new T[Length];
			uint32 i = 0;
			Node *current = Root;

			if (current == nullptr)
				return nullptr;
			
			while (current != nullptr)
				{
				arr[i] = current->Value;
				++i;
				current = current->Next;
				}

			return arr;
			}

		void Print()
			{
			if (Length == 0)
				{
				SAFE_LOG_WARN("List was empty!");
				return;
				}

			Node *current = Root;
			if (current == nullptr)
				return;

			while (current != nullptr)
				{
				std::cout << current->Value << std::endl;
				current = current->Next;
				}
			}

		friend std::ostream &operator<<(std::ostream &stream, SafeList<T> &list)
			{
			Node *current = list.Root;
			uint32 i = 0;

			if (current == nullptr)
				return stream;

			stream << "[";
			while (current != nullptr)
				{
				if ((i + 1) == list.Size())
					stream << current->Value;
				else
					stream << current->Value << ", ";

				++i;
				current = current->Next;
				}
			stream << "]";

			return stream;
			}
	};
