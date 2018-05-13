//	Copyright (C) 2014, 2018 Vaptistis Anogeianakis <nomad@cornercase.gr>
/*
 *	This file is part of LostArt.
 *
 *	LostArt is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	LostArt is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with LostArt.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DSEL_H
#define DSEL_H

namespace graphene
{
	// Forward class declarations
	namespace Frames
	{
		namespace Renderable
		{
			template<typename BaseType, typename TrueWrapper, typename FalseWrapper, typename UnaryPredicate> class Conditional;
			template<typename BaseType, typename FirstWrapper, typename SecondWrapper> class Sequential;
		} // end namespace Renderable
	} // end namespace Frames

	/** This is a DSEL for composing frames to create frame stacks.
	 *  The syntax is far from ideal due to limitations of the C++ language and current implementations,
	 *  but should be a noticeable improvement compared to composing them by hand.
	 */
	namespace DSEL
	{
		// TODO: Create a real DSEL using overloaded operators and/or macros. Include conditionals and sequences.

		/**
		 * Wraps a single frame with its parameters.
		 */
		template<template<typename...> class Template, typename... Arguments>
		struct Frame {};

		/**
		 * Wraps a conditional frame with its parameters.
		 */
		template<typename UnaryPredicate, typename TrueFrames, typename FalseFrames>
		struct Condition {};

		/**
		 * Represents a renderable frame composed of a number of smaller renderable frames rendered in sequence.
		 *
		 * Only makes sense for 2 or more frame arguments.
		 */
		template<typename... Frames>
		struct Sequence {};

		/**
		 * Represents a complete frame stack.
		 */
		template<typename BaseType, typename... Frames>
		struct FrameStack;

		namespace Helpers
		{
			/**
			 * Transforms a wrapper type (Frame, Condition, Sequence, FrameStack) to an actual frame substituting BaseType.
			 */
			template<typename BaseType, typename DSELType>
			struct EvalDSELType;

			/**
			* Takes care of nesting frames and filling-in default arguments.
			*/
			template<typename... Frames>
			struct ProcessFrames;

			template<typename BaseType, template<typename...> class Template, typename... Arguments>
			struct EvalDSELType<BaseType, Frame<Template, Arguments...>>
			{using type = Template<BaseType, Arguments...>;};

			template<typename BaseType, typename... Frames>
			struct EvalDSELType<BaseType, FrameStack<Frames...>>
			{using type = typename ProcessFrames<BaseType, Frames...>::type;};

			template<typename BaseType, typename UnaryPredicate, typename TrueFrames, typename FalseFrames>
			struct EvalDSELType<BaseType, Condition<UnaryPredicate, TrueFrames, FalseFrames>>
			{using type = Frames::Renderable::Conditional<
							BaseType,
							typename EvalDSELType<BaseType,TrueFrames>::type,
							typename EvalDSELType<BaseType,FalseFrames>::type,
							UnaryPredicate>;
			};

			// recursion base
			template<typename BaseType, typename FirstFrame, typename SecondFrame>
			struct EvalDSELType<BaseType, Sequence<Frames::Renderable::Sequential<BaseType,FirstFrame,SecondFrame>>>
			{using type = Frames::Renderable::Sequential<BaseType,FirstFrame,SecondFrame>;};

			// recursion step
			template<typename BaseType, typename FirstFrame, typename SecondFrame, typename... MoreFrames>
			struct EvalDSELType<BaseType, Sequence<FirstFrame, SecondFrame, MoreFrames...>>
			{using type = typename EvalDSELType<BaseType, Sequence<
				Frames::Renderable::Sequential<
					BaseType,
					typename EvalDSELType<BaseType, FirstFrame>::type,
					typename EvalDSELType<BaseType, SecondFrame>::type>,
				MoreFrames...>>::type;
			};

			// recursion base
			template<typename BaseType>
			struct ProcessFrames<BaseType>
			{using type = BaseType;};

			// recursion step
			template<typename BaseType, typename FirstFrame, typename... MoreFrames>
			struct ProcessFrames<BaseType, FirstFrame, MoreFrames...>
			{using type = typename ProcessFrames<typename EvalDSELType<BaseType, FirstFrame>::type, MoreFrames...>::type;};

			template<typename BaseType, typename FrameStackType>
			struct InjectFrameStackType : BaseType
			{
				// member types
				using base_type = BaseType;
				using frame_stack_type = FrameStackType;

				// Constructors
				InjectFrameStackType() = default;
				using base_type::base_type;
			};
		} // end namespace Helpers

		/**
		 * Represents a complete frame stack.
		 */
		template<typename BaseType, typename... Frames>
		struct FrameStack : Helpers::ProcessFrames<Helpers::InjectFrameStackType<BaseType, FrameStack<BaseType, Frames...>>, Frames...>::type
		{
			// Member types
			using base_type = typename Helpers::ProcessFrames<Helpers::InjectFrameStackType<BaseType, FrameStack<BaseType, Frames...>>, Frames...>::type;

			// Constructors
			FrameStack() = default;
			using base_type::base_type;
		}; // end struct FrameStack

	} // end namespace DSEL

} // end namespace graphene

#endif // DSEL_H
