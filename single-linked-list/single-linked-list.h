#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include <vector>


template <typename Type>
class SingleLinkedList {

	struct Node {
		Node() = default;
		Node(const Type& val, Node* next)
			: value(val)
			, next_node(next) {
		}
		Type value;
		Node* next_node = nullptr;
	};

	template <typename ValueType>
	class BasicIterator {

		friend class SingleLinkedList;

		explicit BasicIterator(Node* node) {
			node_ = node;
		}

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = Type;
		using difference_type = std::ptrdiff_t;
		using pointer = ValueType*;
		using reference = ValueType&;

		BasicIterator() = default;

		BasicIterator(const BasicIterator<Type>& other) noexcept {
			node_ = other.node_;
		}

		BasicIterator& operator=(const BasicIterator& rhs) = default;

		[[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
			return node_ == rhs.node_;
		}

		[[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
			return !(node_ == rhs.node_);
		}

		[[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
			return node_ == rhs.node_;
		}

		[[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
			return !(node_ == rhs.node_);
		}

		BasicIterator& operator++() noexcept {
			assert(node_ != nullptr);
			node_ = node_->next_node;
			return *this;
		}

		BasicIterator operator++(int) noexcept {
			BasicIterator tmp(node_);
			node_ = node_->next_node;
			return tmp;
		}

		[[nodiscard]] reference operator*() const noexcept {
			assert(node_ != nullptr);
			return node_->value;
		}

		[[nodiscard]] pointer operator->() const noexcept {
			assert(node_ != nullptr);
			return &(node_->value);
		}

	private:
		Node* node_ = nullptr;
	};

public:
	using value_type = Type;
	using reference = value_type&;
	using const_reference = const value_type&;

	using Iterator = BasicIterator<Type>;

	using ConstIterator = BasicIterator<const Type>;

	[[nodiscard]] Iterator begin() noexcept {
		return Iterator{ head_.next_node };
	}

	[[nodiscard]] Iterator end() noexcept {
		return Iterator(nullptr);
	}

	[[nodiscard]] ConstIterator begin() const noexcept {
		ConstIterator begin(head_.next_node);
		return begin;
	}

	[[nodiscard]] ConstIterator end() const noexcept {
		return ConstIterator(nullptr);
	}

	[[nodiscard]] ConstIterator cbegin() const noexcept {
		return begin();
	}

	[[nodiscard]] ConstIterator cend() const noexcept {
		return end();
	}

	[[nodiscard]] Iterator before_begin() noexcept {
		return Iterator(&head_);
	}

	[[nodiscard]] ConstIterator cbefore_begin() const noexcept {
		return ConstIterator(const_cast<Node*> (&head_));
	}

	[[nodiscard]] ConstIterator before_begin() const noexcept {
		return ConstIterator(const_cast<Node*> (&head_));
	}

public:
	SingleLinkedList()
		: head_()
		, size_(0) {
	}
	SingleLinkedList(std::initializer_list<Type> values) :head_(), size_(0) {
		Init(values);
	}

	SingleLinkedList(const SingleLinkedList& other) :head_(), size_(0) {
		Init(other);
	}
	
	template <typename Container>
	void Init (Container& container) {
		SingleLinkedList temp;
		auto t = temp.before_begin();
		for (auto& item : container) {
		    t = temp.InsertAfter(t, item);
		}
		swap(temp);
	}

	SingleLinkedList& operator=(const SingleLinkedList& rhs) {
		if (this == &rhs) {
			return *this;
		}
		SingleLinkedList temp(rhs);
		swap(temp);
		return *this;
	}

	void swap(SingleLinkedList& other) noexcept {
		std::swap(other.size_, size_);
		std::swap(other.head_.next_node, head_.next_node);
	}

	[[nodiscard]] size_t GetSize() const noexcept {
		return size_;
	}

	[[nodiscard]] bool IsEmpty() const noexcept {
		return head_.next_node == nullptr;
	}
	
	void PushFront(const Type& value) {
		head_.next_node = new Node(value, head_.next_node);
		++size_;
	}

	Iterator InsertAfter(ConstIterator pos, const Type& value) {
		assert(pos.node_ != nullptr);
		if (pos != this->before_begin()) {
			Node* new_node = new Node(value, pos.node_->next_node);
			pos.node_->next_node = new_node;
			++size_;
			return Iterator(new_node);
		}
		else {
			PushFront(value);
			return Iterator(head_.next_node);
		}
	}

	void PopFront() noexcept {
		assert(head_.next_node == nullptr);
        Node* before_node = head_.next_node;
        head_.next_node = before_node->next_node;
        --size_;
        delete before_node;

	}

	Iterator EraseAfter(ConstIterator pos) noexcept {
		assert(pos.node_ != nullptr);
		Node* before = pos.node_;
		auto after_item = before->next_node->next_node;
		delete before->next_node;
		before->next_node = after_item;
		--this->size_;
		return Iterator(before->next_node);
	}

	~SingleLinkedList() {
		Clear();
	}

	void Clear() noexcept {
		while (head_.next_node != nullptr) {
			Node* temp = head_.next_node;
			head_.next_node = head_.next_node->next_node;
			delete temp;
		}
		size_ = 0;
	}

private:
	Node head_;
	size_t size_ = 0;
};

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return !(lhs > rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return !(lhs < rhs);
}

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
	lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
		return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return !(lhs == rhs);
}
