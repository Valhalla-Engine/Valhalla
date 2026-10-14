module;

#include <type_traits>
#include <atomic>

export module Valhalla.Core.Ref;

import Valhalla.Core.Types;



export namespace Valhalla
{
	export class RefCounted
	{
	public:

		void IncRefCount() const
		{
			m_RefCount.fetch_add(1, std::memory_order_relaxed);
		}

		bool DecRefCount() const
		{
			return m_RefCount.fetch_sub(1, std::memory_order_acq_rel) == 1;
		}

		uint32_t GetRefCount() const { return m_RefCount.load(std::memory_order_acquire); }

	private:
		mutable std::atomic<uint32_t> m_RefCount{0};
	};

	export template<typename T>
	class Ref
	{
	public:
		Ref()
			: m_Instance(nullptr) {}

		Ref(std::nullptr_t n)
			: m_Instance(nullptr) {}

		Ref(T* instance)
			: m_Instance(instance)
		{
			static_assert(std::is_base_of<RefCounted, T>::value, "Class is not RefCounted");

			IncRef();
		}

		template<typename T2>
		Ref(const Ref<T2>& other)
		{
			m_Instance = (T*)other.m_Instance;
			IncRef();
		}

		template<typename T2>
		Ref(Ref<T2>&& other)
		{
			m_Instance = (T*)other.m_Instance;
			other.m_Instance = nullptr;
		}

		~Ref()
		{
			DecRef();
		}

		Ref(const Ref<T>& other)
			: m_Instance(other.m_Instance)
		{
			IncRef();
		}

		Ref& operator=(std::nullptr_t)
		{
			DecRef();
			m_Instance = nullptr;
			return *this;
		}

		Ref& operator=(Ref<T>& other)
		{
			other.IncRef();
			DecRef();

			m_Instance = other.m_Instance;
			return *this;
		}

		template<typename T2>
		Ref& operator=(const Ref<T2>& other)
		{
			if (this == &other)
				return *this;

			other.IncRef();
			DecRef();

			m_Instance = other.m_Instance;
			return *this;
		}

		template<typename T2>
		Ref& operator=(Ref<T2>&& other)
		{
			DecRef();

			m_Instance = other.m_Instance;
			other.m_Instance = nullptr;
			return *this;
		}
		
		operator bool() { return m_Instance != nullptr; }
		operator bool() const { return m_Instance != nullptr; }

		T* operator->()
		{
			return m_Instance;
		}

		T* operator->() const
		{
			return m_Instance;
		}

		T& operator*()
		{
			return *m_Instance;
		}

		T& operator*() const
		{
			return *m_Instance;
		}

		T* Raw() { return m_Instance; }
		const T* Raw() const { return m_Instance; }

		void Reset(T* instance = nullptr)
		{
			if (m_Instance == instance)
				return;

			DecRef();
			m_Instance = instance;
			IncRef();
		}

		template<typename T2>
		Ref<T2> As() const
		{
			T2* instance = dynamic_cast<T2*>(m_Instance);
			return Ref<T2>(instance);
		}

		template<typename ... Args>
		static Ref<T>Create(Args&& ... args)
		{
			return Ref<T>(new T(std::forward<Args>(args)...));
		}
	private:

		void IncRef() const
		{
			if (m_Instance)
				m_Instance->IncRefCount();
		}

		void DecRef() const
		{
			if (m_Instance && m_Instance->DecRefCount())
			{
				delete m_Instance;
			}
		}

		template<class T2>
		friend class Ref;
		T* m_Instance;
	};
}