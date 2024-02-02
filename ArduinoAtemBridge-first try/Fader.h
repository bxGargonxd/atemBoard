#pragma once
class Fader
{
public:
  	/**
	 * Constructor.
	*/
	Fader() noexcept;

	/**
	 * Getter for the fader value
	 * \return value of the fader
  	*/
	double get_value() noexcept { return value; };

	/**
	 * Setter for the fader value
	 * \param in new fader value
  	*/
	void set_value(double in) noexcept { value = in; };

	/**
	 * Getter for the fader start value
	 * \return start value
  	*/
	double get_start() noexcept { return start; };

	/**
	 * Setter for the fader start value
	 * \param in New fader start value
  	*/
	void set_start(double in) noexcept { start = in; };

	/**
	 * Getter for the active state
	 * \return active state
  	*/
	bool get_active() noexcept { return active; };

    /**
     * Setter for the active state
     * \param in active state
	*/
	void set_active(bool in) noexcept { active = in; };

private:
	bool active;
	double start;
	double value;
};

