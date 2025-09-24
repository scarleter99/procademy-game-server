#pragma once

class BaseObject
{
public:
	bool GetDelete() { return _delete; }
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	int _x = 0;
	bool _delete = false;
};

class OneStar : public BaseObject
{
	void Update() override;
	void Render() override;
};

class TwoStar : public BaseObject
{
	void Update() override;
	void Render() override;
};

class ThreeStar : public BaseObject
{
	void Update() override;
	void Render() override;
};