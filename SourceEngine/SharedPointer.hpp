#ifndef SHARED_POINTER_HPP
#define SHARED_POINTER_HPP

template<typename T>
class sp {
public:
	sp()
	{
		mRef = 0;
	}

	sp(T *p)
	{
		mRef = new Ref;
		mRef->p = p;
		mRef->refCount = 1;
	}

	sp(const sp<T> &other)
	{
		mRef = other.mRef;
		acquire();
	}
	~sp()
	{
		release();
	}

	sp<T> &operator=(const sp<T> &other)
	{
		release();
		mRef = other.mRef;
		acquire();

		return *this;
	}

	T *operator*()
	{
		return deref();
	}

	T *operator->()
	{
		return deref();
	}

	operator bool()
	{
		return mRef != 0;
	}

private:
	struct Ref {
		T *p;
		int refCount;
	};

	void acquire() {
		if(mRef) {
			mRef->refCount++;
		}
	}

	void release() {
		if(mRef) {
			mRef->refCount--;
			if(mRef->refCount == 0) {
				if(mRef->p) {
					delete mRef->p;
				}
				delete mRef;
			}
		}
	}

	T *deref() {
		return mRef ? mRef->p : 0;
	}

	Ref *mRef;
};

#endif