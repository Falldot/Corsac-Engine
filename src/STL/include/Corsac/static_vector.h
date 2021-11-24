//
// Created by Falldot on 20.11.2021.
//

#ifndef CORSAC_STATIC_VECTOR_H
#define CORSAC_STATIC_VECTOR_H

#pragma once

namespace corsac
{

/// Implements a set via a random access container such as a vector.
/// This container is also known as a sorted_vector. We choose to call it
/// vector_set, as that is a more consistent universally applicable name
/// for it in this library.
///
/// Note that with vector_set, vector_multiset, vector_map, vector_multimap
/// that the modification of the container potentially invalidates all
/// existing iterators into the container, unlike what happens with conventional
/// sets and maps.
///
/// To consider: std::set has the limitation that values in the set cannot
/// be modified, with the idea that modifying them would change their sort
/// order. We have the opportunity to make it so that values can be modified
/// via changing iterators to be non-const, with the downside being that
/// the container can get screwed up if the user screws up. Alternatively,
/// we can do what std STL does and require the user to make their stored
/// classes use 'mutable' as needed. See the C++ standard defect report
/// #103 (DR 103) for a discussion of this.
///
/// Note that the erase functions return iterator and not void. This allows for
/// more efficient use of the container and is consistent with the C++ language
/// defect report #130 (DR 130)

#endif //CORSAC_STATIC_VECTOR_H
