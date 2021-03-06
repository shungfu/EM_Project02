#include "Equation.h"

Equation::Equation()
{
}

std::vector<std::string> postfix(std::string str) {
	std::vector<char> stack;
	std::vector<std::string> postfix;
	std::string buff = "";
	for (unsigned int i = 0; i < str.length(); i++) {
		if (i == 0 && str[i] == '-') {
			buff += '-';
		}
		else if (i != 0 && (priority(str[i - 1]) > 0) && str[i] == '-') {
			buff += '-';
		}
		else {
			if (str[i] == '(' || str[i] == ')') {
				if (buff != "") {
					postfix.push_back(buff);
					if (buff == "sin" || buff == "cos" || buff == "tan" || buff == "csc" || buff == "sec" || buff == "tan") {
						stack.push_back('$');
					}
					buff = "";
				}
				if (str[i] == '(') {
					stack.push_back(str[i]);
					continue;
				}
				else {
					while (stack[stack.size() - 1] != '(') {
						buff += stack[stack.size() - 1];
						stack.pop_back();
						postfix.push_back(buff);
						buff = "";
					}
					stack.pop_back();
					if (stack[stack.size() - 1] == '$') {
						buff += stack[stack.size() - 1];
						stack.pop_back();
						postfix.push_back(buff);
						buff = "";
					}
					continue;
				}
			}
			if (str[i] == '-' || str[i] == '+' || str[i] == '*' || str[i] == '^') {
				if (buff != "") {
					postfix.push_back(buff);
					buff = "";
				}
				while ((stack.size() > 0) && (priority(stack[stack.size() - 1]) >= priority(str[i]))) {
					buff += stack[stack.size() - 1];
					stack.pop_back();
					postfix.push_back(buff);
					buff = "";
				}
				stack.push_back(str[i]);
			}
			else {
				buff += str[i];
			}
		}
	}
	while (stack.size() > 0) {
		if (buff != "") {
			postfix.push_back(buff);
			buff = "";
		}
		buff += stack[stack.size() - 1];
		stack.pop_back();
		postfix.push_back(buff);
		buff = "";
	}
	return postfix;
}

std::vector<double> Equation::sorting(std::vector<double> v) {
	double temp;
	for (unsigned int i = 0; i < v.size(); i++) {
		for (unsigned int j = i + 1; j < v.size(); j++) {
			if (v[j] < v[i]) {
				temp = v[j];
				v[j] = v[i];
				v[i] = temp;
			}
		}
	}
	return v;
}

int priority(char op) {
	switch (op) {
	case'$':
		return 4;
	case '^':
		return 3;
	case '*':
		return 2;
	case '+':case '-':
		return 1;
	default:
		return 0;
	}
}

double compare(double max, double min, double x) {
	if (x > max) {
		return max;
	}
	else if (x < min) {
		return min;
	}
	else {
		return x;
	}
}

void Equation::SetEquation(std::vector<std::string>equ)
{
	equation = equ;

	// calculate dim
	bool meetX = false, meetY = false;
	for (int i = 0; i < equation.size(); i++) {
		if (equation[i] == "x" && !meetX)
			meetX = true;
		else if (equation[i] == "y" && !meetY)
			meetY = true;

		if (meetX && meetY) break; // Terminate Earlier if meet both XY
	}

	if (meetX && meetY) {
		dim = 3;
	}
	else if (meetY) {
		dim = 2;
	}
	else if (meetX) {
		dim = 1;
	}
	else { // No meet
		dim = 0;
	}
}

double Equation::goldenSectionSearch(double a, double b, double c, Vector S) {
	double x;
	if (c - b > b - a) {
		x = b + resphi * (c - b);
	}
	else {
		x = b - resphi * (b - a);
	}
	if (abs(c - a) <= threshold * (abs(b) + abs(x))) {
		return (c + a) / 2;
	}
	double fx, fb;
	if (dim == 1) {
		fx = f(this->X.Data[0] + x * S.Data[0], 0);
		fb = f(this->X.Data[0] + b * S.Data[0], 0);
	}
	else if (dim == 2) {
		fx = f(0, this->X.Data[1] + x * S.Data[1]);
		fb = f(0, this->X.Data[1] + b * S.Data[1]);
	}
	else {
		fb = f(this->X.Data[0] + b * S.Data[0], this->X.Data[1] + b * S.Data[1]);
		fx = f(this->X.Data[0] + x * S.Data[0], this->X.Data[1] + x * S.Data[1]);
	}
	if (fx < fb && !isnan(fx)) {
		if (c - b > b - a) return goldenSectionSearch(b, x, c, S);
		else return goldenSectionSearch(a, x, b, S);
	}
	else {
		if (c - b > b - a) return goldenSectionSearch(a, b, x, S);
		else return goldenSectionSearch(x, b, c, S);
	}
}

double Equation::f(double x, double y) {
	std::vector<double> var;
	double buff = 0;
	std::string temp = "";
	for (unsigned int i = 0; i < equation.size(); i++) {
		if (equation[i] == "x") {
			var.push_back(x);
		}
		else if (equation[i] == "y") {
			var.push_back(y);
		}
		else if (equation[i] == "+") {
			var[var.size() - 2] += var[var.size() - 1];
			var.pop_back();
		}
		else if (equation[i] == "-") {
			var[var.size() - 2] -= var[var.size() - 1];
			var.pop_back();
		}
		else if (equation[i] == "*") {
			var[var.size() - 2] *= var[var.size() - 1];
			var.pop_back();
		}
		else if (equation[i] == "^") {
			var[var.size() - 2] = pow(var[var.size() - 2], var[var.size() - 1]);
			var.pop_back();
		}
		else if (equation[i] == "sin" || equation[i] == "cos" || equation[i] == "tan" || equation[i] == "sec" || equation[i] == "csc" || equation[i] == "cot") {
			temp = equation[i];
		}
		else if (equation[i] == "$") {
			if (temp == "sin") {
				var[var.size() - 1] = sin(var[var.size() - 1]);
			}
			else if (temp == "cos") {
				var[var.size() - 1] = cos(var[var.size() - 1]);
			}
			else if (temp == "tan") {
				var[var.size() - 1] = tan(var[var.size() - 1]);
			}
			else if (temp == "sec") {
				var[var.size() - 1] = 1 / cos(var[var.size() - 1]);
			}
			else if (temp == "csc") {
				var[var.size() - 1] = 1 / sin(var[var.size() - 1]);
			}
			else if (temp == "cot") {
				var[var.size() - 1] = 1 / tan(var[var.size() - 1]);
			}
		}
		else {
			var.push_back(stod(equation[i]));
		}
	}
	if (var.size() == 1) {
		return var[0];
	}
}

double Equation::f(const Vector& xy)
{
	std::vector<double> var;
	double buff = 0;
	std::string temp = "";
	for (unsigned int i = 0; i < equation.size(); i++) {
		if (equation[i] == "x") {
			var.push_back(xy.Data[0]);
		}
		else if (equation[i] == "y") {
			var.push_back(xy.Data[1]);
		}
		else if (equation[i] == "+") {
			var[var.size() - 2] += var[var.size() - 1];
			var.pop_back();
		}
		else if (equation[i] == "-") {
			var[var.size() - 2] -= var[var.size() - 1];
			var.pop_back();
		}
		else if (equation[i] == "*") {
			var[var.size() - 2] *= var[var.size() - 1];
			var.pop_back();
		}
		else if (equation[i] == "^") {
			var[var.size() - 2] = pow(var[var.size() - 2], var[var.size() - 1]);
			var.pop_back();
		}
		else if (equation[i] == "sin" || equation[i] == "cos" || equation[i] == "tan" || equation[i] == "sec" || equation[i] == "csc" || equation[i] == "cot") {
			temp = equation[i];
		}
		else if (equation[i] == "$") {
			if (temp == "sin") {
				var[var.size() - 1] = sin(var[var.size() - 1]);
			}
			else if (temp == "cos") {
				var[var.size() - 1] = cos(var[var.size() - 1]);
			}
			else if (temp == "tan") {
				var[var.size() - 1] = tan(var[var.size() - 1]);
			}
			else if (temp == "sec") {
				var[var.size() - 1] = 1 / cos(var[var.size() - 1]);
			}
			else if (temp == "csc") {
				var[var.size() - 1] = 1 / sin(var[var.size() - 1]);
			}
			else if (temp == "cot") {
				var[var.size() - 1] = 1 / tan(var[var.size() - 1]);
			}
		}
		else {
			var.push_back(stod(equation[i]));
		}
	}
	if (var.size() == 1) {
		return var[0];
	}
}

double Equation::dX(double x, double y) {
	double f1 = f(x + threshold, y);
	double f2 = f(x - threshold, y);
	double d = (f1 - f2) / (2 * threshold);

	return d;
	// return ((f(x + threshold, y) - f(x - threshold, y)) / (2 * threshold));
}

double Equation::dY(double x, double y) {
	return ((f(x, y + threshold) - f(x, y - threshold)) / (2 * threshold));
}

Matrix Equation::Gradient(const Vector& X)
{
	Matrix gradient;
	Vector d;

	if (dim == 3) {	// XY
		d.Data.push_back(this->dX(X.Data[0], X.Data[1]));
		d.Data.push_back(this->dY(X.Data[0], X.Data[1]));
	}
	else if (dim == 2) {	// Y
		d.Data.push_back(this->dY(0, X.Data[0]));
	}
	else if (dim == 1) {	// X
		d.Data.push_back(this->dX(X.Data[0], 0));
	}

	gradient.Data.push_back(d);
	return gradient;
}

Matrix Equation::Hessian(const Vector& X)
{
	Matrix hessian;

	if (dim == 3) {	// XY
		double dXX = 0, dYX = 0, dXY = 0, dYY = 0;
		Vector row1, row2;
		dXX = (dX(X.Data[0] + threshold, X.Data[1]) - dX(X.Data[0] - threshold, X.Data[1])) / (2 * threshold);
		dYX = (dY(X.Data[0] + threshold, X.Data[1]) - dY(X.Data[0] - threshold, X.Data[1])) / (2 * threshold);
		dXY = (dX(X.Data[0], X.Data[1] + threshold) - dX(X.Data[0], X.Data[1] - threshold)) / (2 * threshold);
		dYY = (dY(X.Data[0], X.Data[1] + threshold) - dY(X.Data[0], X.Data[1] - threshold)) / (2 * threshold);
		row1.Data.push_back(dXX);
		row1.Data.push_back(dYX);
		row2.Data.push_back(dXY);
		row2.Data.push_back(dYY);
		hessian.Data.push_back(row1);
		hessian.Data.push_back(row2);
	}
	else if (dim == 2) {	// Y
		Vector row1;
		double dYY = 0;
		dYY = (dY(0, X.Data[0] + threshold) - dY(0, X.Data[0] - threshold)) / (2 * threshold);
		row1.Data.push_back(dYY);
		hessian.Data.push_back(row1);
	}
	else if (dim == 1) {	// X
		Vector row1;
		double dXX = 0;
		dXX = (dX(X.Data[0] + threshold, 0) - dX(X.Data[0] - threshold, 0)) / (2 * threshold);
		row1.Data.push_back(dXX);
		hessian.Data.push_back(row1);
	}

	return hessian;
}

void Equation::Powell(double x, double y, double xMin, double xMax, double yMin, double yMax, System::Windows::Forms::TextBox ^ Output)
{
	int i = 1, j = 1;
	std::vector<Vector> S;

	if (dim == 1) {
		this->X.Data.push_back(x);
		this->X.Data.push_back(0);
		Vector temp;
		// S1
		temp.Data.push_back(1);
		temp.Data.push_back(0);
		S.push_back(temp);
		temp.Data.clear();
		// S2
		temp.Data.push_back(0);
		temp.Data.push_back(0);
		S.push_back(temp);
		temp.Data.clear();
		double x2, GSmin, GSmax;
		double alpha1, alpha2;
		int cnt = 0;
		for (;;) {
			Output->Text += "j = " + j + System::Environment::NewLine;
			if (j != 1) {
				S[0].Data[0] = S[1].Data[0];
			}
			Output->Text += "i = " + i + System::Environment::NewLine;
			Output->Text += "X1 = [ " + this->X.Data[0] + " ]" + System::Environment::NewLine;
			GSmax = (xMax - this->X.Data[0]) / S[0].Data[0];
			GSmin = (xMin - this->X.Data[0]) / S[0].Data[0];
			if (GSmax < GSmin) {
				double temp = GSmax;
				GSmax = GSmin;
				GSmin = temp;
			}
			alpha1 = goldenSectionSearch(GSmin, (GSmax + GSmin) / 2, GSmax, S[0]);
			x2 = this->X.Data[0] + alpha1 * S[0].Data[0];
			Output->Text += "alpha = " + alpha1 + System::Environment::NewLine;
			x2 = compare(xMax, xMin, x2);
			if (abs(x2 - this->X.Data[0]) < threshold) break;
			this->X.Data[0] = x2;
			Output->Text += "X2 = [ " + this->X.Data[0] + " ]" + System::Environment::NewLine;
			++i;
			S[1].Data[0] = alpha1 * S[0].Data[0];
			Output->Text += "i = " + i + System::Environment::NewLine;
			Output->Text += "X2 = [ " + this->X.Data[0] + " ]" + System::Environment::NewLine;
			GSmax = (xMax - this->X.Data[0]) / S[1].Data[0];
			GSmin = (xMin - this->X.Data[0]) / S[1].Data[0];
			if (GSmax < GSmin) {
				double temp = GSmax;
				GSmax = GSmin;
				GSmin = temp;
			}
			alpha2 = goldenSectionSearch(GSmin, (GSmax + GSmin) / 2, GSmax, S[1]);
			x2 = this->X.Data[0] + alpha2 * S[1].Data[0];
			Output->Text += "alpha = " + alpha2 + System::Environment::NewLine;
			x2 = compare(xMax, xMin, x2);
			Output->Text += "S2 = [ " + S[1].Data[0] + " ]" + System::Environment::NewLine;
			Output->Text += "X3 = [ " + x2 + " ]" + System::Environment::NewLine;
			if (abs(x2 - this->X.Data[0]) < threshold) break;
			this->X.Data[0] = x2;
			i = 1;
			++j;
		}
		Output->Text += "X = [ " + x2 + " ]" + System::Environment::NewLine;
		Output->Text += "min = " + f(x2, 0) + System::Environment::NewLine;
	}
	else if (dim == 2) {
		this->X.Data.push_back(0);
		this->X.Data.push_back(y);
		Vector temp;
		// S1
		temp.Data.push_back(0);
		temp.Data.push_back(1);
		S.push_back(temp);
		temp.Data.clear();
		// S2
		temp.Data.push_back(0);
		temp.Data.push_back(0);
		S.push_back(temp);
		temp.Data.clear();
		double y2, GSmin, GSmax;
		double alpha1, alpha2;
		for (;;) {
			Output->Text += "j = " + j + System::Environment::NewLine;
			if (j != 1) {
				S[0].Data[1] = S[1].Data[1];
			}
			Output->Text += "i = " + i + System::Environment::NewLine;
			Output->Text += "X1 = [ " + this->X.Data[1] + " ]" + System::Environment::NewLine;
			GSmax = (yMax - this->X.Data[1]) / S[0].Data[1];
			GSmin = (yMin - this->X.Data[1]) / S[0].Data[1];
			if (GSmax < GSmin) {
				double temp = GSmax;
				GSmax = GSmin;
				GSmin = temp;
			}
			alpha1 = goldenSectionSearch(GSmin, (GSmax + GSmin) / 2, GSmax, S[0]);
			Output->Text += "alpha = " + alpha1 + System::Environment::NewLine;
			y2 = this->X.Data[1] + alpha1 * S[0].Data[1];
			y2 = compare(yMax, yMin, y2);
			if (abs(y2 - this->X.Data[1]) <= threshold) {
				break;
			}
			this->X.Data[1] = y2;
			Output->Text += "X2 = [ " + this->X.Data[1] + " ]" + System::Environment::NewLine;
			++i;
			S[1].Data[1] = alpha1 * S[0].Data[1];
			Output->Text += "i = " + i + System::Environment::NewLine;
			Output->Text += "X2 = [ " + this->X.Data[1] + " ]" + System::Environment::NewLine;
			GSmax = (yMax - this->X.Data[1]) / S[1].Data[1];
			GSmin = (yMin - this->X.Data[1]) / S[1].Data[1];
			if (GSmax < GSmin) {
				double temp = GSmax;
				GSmax = GSmin;
				GSmin = temp;
			}
			alpha2 = goldenSectionSearch(GSmin, (GSmax + GSmin) / 2, GSmax, S[1]);
			Output->Text += "alpha = " + alpha2 + System::Environment::NewLine;
			y2 = this->X.Data[1] + alpha2 * S[1].Data[1];
			y2 = compare(yMax, yMin, y2);
			Output->Text += "S2 = [ " + S[1].Data[1] + " ]" + System::Environment::NewLine;
			Output->Text += "X3 = [ " + y2 + " ]" + System::Environment::NewLine;
			if (abs(y2 - this->X.Data[1]) <= threshold) {
				break;
			}
			this->X.Data[1] = y2;
			i = 1;
			++j;
		}
		Output->Text += "X = [ " + y2 + " ]" + System::Environment::NewLine;
		Output->Text += "min = " + f(0, y2) + System::Environment::NewLine;
	}
	else if (dim == 3) {
		// initial x , y
		this->X.Data.push_back(x);
		this->X.Data.push_back(y);
		// initial
		Vector temp;
		// S1
		temp.Data.push_back(1);
		temp.Data.push_back(0);
		S.push_back(temp);
		temp.Data.clear();
		// S2
		temp.Data.push_back(0);
		temp.Data.push_back(1);
		S.push_back(temp);
		temp.Data.clear();
		// S3
		temp.Data.push_back(0);
		temp.Data.push_back(0);
		S.push_back(temp);
		temp.Data.clear();
		double x2, y2;
		double GSmin, GSmax;
		double alpha1, alpha2, alpha3;
		int sinorbo = 0;
		std::vector<double> GSsort;
		for (;;) {
			Output->Text += "j = " + j + System::Environment::NewLine;
			if (j != 1) {
				S[0].Data[0] = S[1].Data[0];
				S[1].Data[0] = S[2].Data[0];
				S[0].Data[1] = S[1].Data[1];
				S[1].Data[1] = S[2].Data[1];
			}
			Output->Text += "i = " + i + System::Environment::NewLine;
			Output->Text += "X1 = [ " + this->X.Data[0] + " , " + this->X.Data[1] + " ]" + System::Environment::NewLine;
			double max, min;
			if (S[0].Data[0] != 0) {
				GSsort.push_back((xMax - this->X.Data[0]) / S[0].Data[0]);
				GSsort.push_back((xMin - this->X.Data[0]) / S[0].Data[0]);
				sinorbo++;
			}
			if (S[0].Data[1] != 0) {
				GSsort.push_back((yMax - this->X.Data[1]) / S[0].Data[1]);
				GSsort.push_back((yMin - this->X.Data[1]) / S[0].Data[1]);
				sinorbo++;
			}
			GSsort = sorting(GSsort);
			if (sinorbo == 2) {
				GSmax = GSsort[2];
				GSmin = GSsort[1];
			}
			else {
				GSmax = GSsort[1];
				GSmin = GSsort[0];
			}
			sinorbo = 0;
			if (GSmax < GSmin) {
				double temp = GSmax;
				GSmax = GSmin;
				GSmin = temp;
			}
			GSsort.clear();
			alpha1 = goldenSectionSearch(GSmin, (GSmin + GSmax) / 2, GSmax, S[0]);
			x2 = this->X.Data[0] + alpha1 * S[0].Data[0];
			y2 = this->X.Data[1] + alpha1 * S[0].Data[1];
			x2 = compare(xMax, xMin, x2);
			y2 = compare(yMax, yMin, y2);
			if (abs(f(x2, y2) - f(this->X.Data[0], this->X.Data[1])) <= threshold && abs(sqrt(pow((x2 - this->X.Data[0]), 2) + pow((y2 - this->X.Data[1]), 2))) <= threshold) {
				break;
			}
			this->X.Data[0] = x2;
			this->X.Data[1] = y2;
			Output->Text += "X2 = [ " + this->X.Data[0] + " , " + this->X.Data[1] + " ]" + System::Environment::NewLine;
			++i;
			Output->Text += "i = " + i + System::Environment::NewLine;
			Output->Text += "X2 = [ " + this->X.Data[0] + " , " + this->X.Data[0] + " ]" + System::Environment::NewLine;
			if (S[1].Data[0] != 0) {
				GSsort.push_back((xMax - this->X.Data[0]) / S[1].Data[0]);
				GSsort.push_back((xMin - this->X.Data[0]) / S[1].Data[0]);
				sinorbo++;
			}
			if (S[1].Data[1] != 0) {
				GSsort.push_back((yMax - this->X.Data[1]) / S[1].Data[1]);
				GSsort.push_back((yMin - this->X.Data[1]) / S[1].Data[1]);
				sinorbo++;
			}
			GSsort = sorting(GSsort);
			if (sinorbo == 2) {
				GSmax = GSsort[2];
				GSmin = GSsort[1];
			}
			else {
				GSmax = GSsort[1];
				GSmin = GSsort[0];
			}
			sinorbo = 0;
			if (GSmax < GSmin) {
				double temp = GSmax;
				GSmax = GSmin;
				GSmin = temp;
			}
			GSsort.clear();
			alpha2 = goldenSectionSearch(GSmin, (GSmin + GSmax) / 2, GSmax, S[1]);
			x2 = this->X.Data[0] + alpha2 * S[1].Data[0];
			y2 = this->X.Data[1] + alpha2 * S[1].Data[1];
			x2 = compare(xMax, xMin, x2);
			y2 = compare(yMax, yMin, y2);
			if (abs(f(x2, y2) - f(this->X.Data[0], this->X.Data[1])) <= threshold && abs(sqrt(pow((x2 - this->X.Data[0]), 2) + pow((y2 - this->X.Data[1]), 2))) <= threshold) {
				break;
			}
			this->X.Data[0] = x2;
			this->X.Data[1] = y2;
			Output->Text += "X3 = [ " + this->X.Data[0] + " , " + this->X.Data[1] + " ]" + System::Environment::NewLine;
			S[2].Data[0] = alpha1 * S[0].Data[0] + alpha2 * S[1].Data[0];
			S[2].Data[1] = alpha1 * S[0].Data[1] + alpha2 * S[1].Data[1];
			GSsort.push_back((xMax - this->X.Data[0]) / S[2].Data[0]);
			GSsort.push_back((xMin - this->X.Data[0]) / S[2].Data[0]);
			GSsort.push_back((yMax - this->X.Data[1]) / S[2].Data[1]);
			GSsort.push_back((yMin - this->X.Data[1]) / S[2].Data[1]);
			GSsort = sorting(GSsort);
			GSmax = GSsort[2];
			GSmin = GSsort[1];
			if (GSmax < GSmin) {
				double temp = GSmax;
				GSmax = GSmin;
				GSmin = temp;
			}
			GSsort.clear();
			alpha3 = goldenSectionSearch(GSmin, (GSmin + GSmax) / 2, GSmax, S[2]);
			Output->Text += "alpha = " + alpha3 + System::Environment::NewLine;
			x2 = this->X.Data[0] + alpha3 * S[2].Data[0];
			y2 = this->X.Data[1] + alpha3 * S[2].Data[1];
			x2 = compare(xMax, xMin, x2);
			y2 = compare(yMax, yMin, y2);
			Output->Text += "S3 = [ " + S[2].Data[0] + " , " + S[2].Data[1] + " ]" + System::Environment::NewLine;
			Output->Text += "X4 = [ " + x2 + " , " + y2 + " ]" + System::Environment::NewLine;
			std::cout << f(x2, y2) << std::endl;
			if (abs(f(x2, y2) - f(this->X.Data[0], this->X.Data[1])) <= threshold && abs(sqrt(pow((x2 - this->X.Data[0]), 2) + pow((y2 - this->X.Data[1]), 2))) <= threshold) {
				break;
			}
			this->X.Data[0] = x2;
			this->X.Data[1] = y2;
			i = 1;
			++j;
		}
		Output->Text += "X = [ " + x2 + " , " + y2 + " ]" + System::Environment::NewLine;
		Output->Text += "min = " + f(x2, y2) + System::Environment::NewLine;
	}
}

void Equation::Steepest_Descent(double x, double y, double xMin, double xMax, double yMin, double yMax, System::Windows::Forms::TextBox^ Output)
{
	double Precision = 0.001;
	int Max_iter = 500;

	// Initial X Vector and Internal Vector
	Vector Max_X, Min_X;
	if (dim == 3) {
		X.initial(x, y);
		Max_X.initial(xMax, yMax);
		Min_X.initial(xMin, yMin);
	}
	else if (dim == 2) {
		X.initial(y);
		Max_X.initial(yMax);
		Min_X.initial(yMin);
	}
	else if (dim == 1) {
		X.initial(x);
		Max_X.initial(xMax);
		Min_X.initial(xMin);
	}

	Vector pre_x = X, now_x = X;
	Vector step;

	// step 0
	int k = 0;
	while (Max_iter-- > 0) {

		// step 1
		Matrix G = Gradient(now_x);
		Vector gradient = G.Data[0];	// Matrix to Vector

		if (Norm(gradient) == 0) {
			break;
		}
		gradient = -1 * gradient;

		// Not A Number Happened
		while (isnan(gradient.Data[0])) {
			step = 0.9 * step;
			now_x = pre_x + step;

			G = Gradient(now_x);
			gradient = G.Data[0];
			gradient = -1 * gradient;
		}
		pre_x = now_x;

		// step 2 : compute step-size lambda

		Matrix L1 = G * Transpose(G);
		Matrix H = Hessian(pre_x);
		Matrix L2 = Inverse(G*H*Transpose(G));

		Matrix L = L1 * L2;
		//Matrix L = G * Transpose(G) * Inverse(G * Hessian(pre_x) * Transpose(G));

		Vector lambda = L.Data[0];
		step = lambda * gradient;

		// step 3
		now_x = pre_x + step;

		while (f(now_x)>f(pre_x)) {
			Matrix I;
			I.identity(now_x);
			H = H + I;

			L2 = Inverse(G*H*Transpose(G));
			L = L1 * L2;
			lambda = L.Data[0];
			step = lambda * gradient;

			// step 3
			now_x = pre_x + step;
		}

		// Edge Dealing
		for (int i = 0; i < now_x.getDim(); i++) {
			while (now_x.Data[i] > Max_X.Data[i] || now_x.Data[i] < Min_X.Data[i]) {
				step = 0.9 * step;
				now_x = pre_x + step;
			}
		}

		// step 1: Stopping criteria
		if (Norm(gradient) <= Precision || abs(Norm(pre_x - now_x)) <= Precision) {
			break;
		}

		// step 4 : output
		k++;

		/* Form print */
		// i
		Output->Text += "i = " + k.ToString() + System::Environment::NewLine;
		// h
		Output->Text += "h =" + System::Environment::NewLine + "[" + System::Environment::NewLine;
		for (int i = 0; i < gradient.getDim(); i++)
			Output->Text += gradient.Data[i].ToString() + System::Environment::NewLine;
		Output->Text += "]" + System::Environment::NewLine;
		// lambda
		Output->Text += "lambda = " + lambda.Data[0].ToString() + System::Environment::NewLine;
		// x
		Output->Text += "X =" + System::Environment::NewLine + "[" + System::Environment::NewLine;
		for (int i = 0; i < now_x.getDim(); i++)
			Output->Text += now_x.Data[i].ToString() + System::Environment::NewLine;
		Output->Text += " ]" + System::Environment::NewLine + System::Environment::NewLine;
	}
	Output->Text += "[";
	for (int i = 0; i < pre_x.getDim(); i++) {
		Output->Text += pre_x.Data[i].ToString() + " , ";
	}
	Output->Text += "]" + System::Environment::NewLine;
	Output->Text += System::Environment::NewLine + "min = " + f(pre_x).ToString() + System::Environment::NewLine + System::Environment::NewLine;
}

void Equation::Conjugate(double x, double y, double xMin, double xMax, double yMin, double yMax, System::Windows::Forms::TextBox ^ Output)
{
	// reference:	https://ch-hsieh.blogspot.com/2014/03/conjugate-direction-methods-2-conjugate.html
	if (dim == 3) {
		X.initial(x, y);
	}
	else if (dim == 2) {
		X.initial(y);
	}
	else if (dim == 1) {
		X.initial(x);
	}
	int i = 0;
	if (dim == 3) {
		double alpha, beta;
		Matrix G = Gradient(X);
		Vector gradient = G.Data[0];
		Matrix H = Hessian(X);
		Vector si, Q;
		si.Data.push_back(-gradient.Data[0]);
		si.Data.push_back(-gradient.Data[1]);
		Q.Data.push_back(si.Data[0] * H.Data[0].Data[0] + si.Data[1] * H.Data[0].Data[1]);
		Q.Data.push_back(si.Data[0] * H.Data[1].Data[0] + si.Data[1] * H.Data[1].Data[1]);
		alpha = -(gradient.Data[0] * si.Data[0] + gradient.Data[1] * si.Data[1]) / (Q.Data[0] * si.Data[0] + Q.Data[1] * si.Data[1]);
		X.Data[0] += alpha * si.Data[0];
		X.Data[1] += alpha * si.Data[1];
		Output->Text += "i = " + i + System::Environment::NewLine;
		Output->Text += "Si = [ " + gradient.Data[0] + " , " + gradient.Data[1] + " ]" + System::Environment::NewLine;
		Output->Text += "alpha = " + alpha + System::Environment::NewLine;
		Output->Text += "Xi = [" + X.Data[0] + " , " + X.Data[1] + " ]" + System::Environment::NewLine;
		Vector preG;
		while (i <= 100)
		{
			X.Data[0] = compare(xMax, xMin, X.Data[0]);
			X.Data[1] = compare(yMax, yMin, X.Data[1]);
			++i;
			preG = gradient;
			G = Gradient(X);
			gradient = G.Data[0];
			while (isnan(gradient.Data[0])) {
				alpha *= 0.9;
				X.Data[0] += alpha * si.Data[0];
				X.Data[1] += alpha * si.Data[1];
				G = Gradient(X);
				gradient = G.Data[0];
			}
			H = Hessian(X);
			beta = (pow(gradient.Data[0], 2) + pow(gradient.Data[1], 2)) / (pow(preG.Data[0], 2) + pow(preG.Data[1], 2));
			si.Data[0] = -gradient.Data[0] + beta * si.Data[0];
			si.Data[1] = -gradient.Data[1] + beta * si.Data[1];
			Q.Data[0] = si.Data[0] * H.Data[0].Data[0] + si.Data[1] * H.Data[0].Data[1];
			Q.Data[1] = si.Data[0] * H.Data[1].Data[0] + si.Data[1] * H.Data[1].Data[1];
			alpha = -(gradient.Data[0] * si.Data[0] + gradient.Data[1] * si.Data[1]) / (Q.Data[0] * si.Data[0] + Q.Data[1] * si.Data[1]);
			if (X.Data[0] + alpha * si.Data[0] < 0 || X.Data[1] + alpha * si.Data[1] < 0) {
				alpha *= 0.9;
			}
			X.Data[0] += alpha * si.Data[0];
			X.Data[1] += alpha * si.Data[1];
			Output->Text += "i = " + i + System::Environment::NewLine;
			Output->Text += "beta = " + beta + System::Environment::NewLine;
			Output->Text += "Si = [ " + gradient.Data[0] + " , " + gradient.Data[1] + " ]" + System::Environment::NewLine;
			Output->Text += "alpha = " + alpha + System::Environment::NewLine;
			Output->Text += "Xi = [" + X.Data[0] + " , " + X.Data[1] + " ]" + System::Environment::NewLine;
			if (abs(gradient.Data[0]) < threshold&&abs(gradient.Data[1]) < threshold) {
				break;
			}
		}
		Output->Text += "X = [" + X.Data[0] + " , " + X.Data[1] + " ]" + System::Environment::NewLine;
		Output->Text += "min = " + f(X) + System::Environment::NewLine;
	}
	else {
		double alpha, beta;
		double gradient, d;
		if (dim == 2) {
			gradient = dY(0, X.Data[0]);
			d = (dY(0, X.Data[0] + threshold) - dY(0, X.Data[0] - threshold)) / (2 * threshold);
		}
		else if (dim == 1) {
			gradient = dX(X.Data[0], 0);
			d = (dX(X.Data[0] + threshold, 0) - dX(X.Data[0] - threshold, 0)) / (2 * threshold);
		}
		double si = -gradient;
		alpha = -(gradient*si) / ((pow(si, 2)*d));
		X.Data[0] += alpha * si;
		Output->Text += "i = " + i + System::Environment::NewLine;
		Output->Text += "Si = [ " + gradient + " ]" + System::Environment::NewLine;
		Output->Text += "alpha = " + alpha + System::Environment::NewLine;
		Output->Text += "Xi = [" + X.Data[0] + " ]" + System::Environment::NewLine;
		double preG;
		while (i <= 100)
		{
			++i;
			preG = gradient;
			if (dim == 2) {
				X.Data[0] = compare(yMax, yMin, X.Data[0]);
				gradient = dY(0, X.Data[0]);
				while (isnan(gradient)) {
					alpha *= 0.9;
					X.Data[0] += alpha;
					gradient = dY(0, X.Data[0]);
				}
				d = (dY(0, X.Data[0] + threshold) - dY(0, X.Data[0] - threshold)) / (2 * threshold);
			}
			else if (dim == 1) {
				X.Data[0] = compare(xMax, xMin, X.Data[0]);
				gradient = dX(X.Data[0], 0);
				while (isnan(gradient)) {
					alpha *= 0.9;
					X.Data[0] += alpha;
					gradient = dX(X.Data[0], 0);
				}
				d = (dX(X.Data[0] + threshold, 0) - dX(X.Data[0] - threshold, 0)) / (2 * threshold);
			}
			beta = pow(gradient, 2) / pow(preG, 2);
			si = -gradient + beta * si;
			alpha = -(gradient*si) / ((pow(si, 2)*d));
			if (X.Data[0] + alpha * si < 0) {
				alpha *= 0.9;
			}
			X.Data[0] += alpha * si;
			Output->Text += "i = " + i + System::Environment::NewLine;
			Output->Text += "beta = " + beta + System::Environment::NewLine;
			Output->Text += "Si = [ " + gradient + " ]" + System::Environment::NewLine;
			Output->Text += "alpha = " + alpha + System::Environment::NewLine;
			Output->Text += "Xi = [" + X.Data[0] + " ]" + System::Environment::NewLine;
			if (abs(gradient) < threshold) {
				break;
			}
		}
		Output->Text += "X = [" + X.Data[0] + " ]" + System::Environment::NewLine;
		if (dim == 2) {
			Output->Text += "min = " + f(0, X.Data[0]) + System::Environment::NewLine;
		}
		else if (dim == 1) {
			Output->Text += "min = " + f(X.Data[0], 0) + System::Environment::NewLine;
		}
	}
}

void Equation::Newton(double x, double y, double xMin, double xMax, double yMin, double yMax, System::Windows::Forms::TextBox ^Output)
{
	double Precision = 0.001;
	int Max_iter = 500;

	// Initial X Vector and Internal Vector
	Vector Max_X, Min_X;
	if (dim == 3) {
		X.initial(x, y);
		Max_X.initial(xMax, yMax);
		Min_X.initial(xMin, yMin);
	}
	else if (dim == 2) {
		X.initial(y);
		Max_X.initial(yMax);
		Min_X.initial(yMin);
	}
	else if (dim == 1) {
		X.initial(x);
		Max_X.initial(xMax);
		Min_X.initial(xMin);
	}

	Vector pre_x = X, now_x = X;
	Vector step;

	int k = 0;
	while (Max_iter-- > 0) {
		
		// Step
		Matrix G = Gradient(now_x);	// Gradient
		Matrix H = Hessian(now_x);	// Hessian

		// Assure Hessian be positive to reach global minium

		Matrix InvH = Inverse(H);	// Inverse Hessian
		Matrix S = G * Transpose(InvH);
		// Not a Number
		while (isnan(S.Data[0].Data[0]) || isnan(G.Data[0].Data[0])) {
			step = step * 0.9;
			now_x = pre_x - step;

			G = Gradient(now_x);
			H = Hessian(now_x);
			InvH = Inverse(H);
			S = Gradient(now_x) * Transpose(InvH);
		}
		step = S.Data[0];

		pre_x = now_x;
		now_x = pre_x - step;

		while (f(now_x) > f(pre_x)) {
			// Assure Hessian be positive to reach global minium
			Matrix I;
			I.identity(now_x);
			H = H + I;

			InvH = Inverse(H);	// Inverse Hessian
			S = G * Transpose(InvH);
			// Not a Number
			while (isnan(S.Data[0].Data[0]) || isnan(G.Data[0].Data[0])) {
				step = step * 0.9;
				now_x = pre_x - step;

				H = Hessian(now_x);
				InvH = Inverse(H);
				S = Gradient(now_x) * Transpose(InvH);
			}
			step = S.Data[0];

			
			now_x = pre_x - step;
		}
		

		// Edge Dealing
		for (int i = 0; i < now_x.getDim(); i++) {
			while (now_x.Data[i] > Max_X.Data[i] || now_x.Data[i] < Min_X.Data[i]) {
				step = 0.9 * step;
				now_x = pre_x - step;
			}
		}

		// Stopping Criteria
		if (abs(Norm(now_x - pre_x)) <= Precision) {
			break;
		}

		k++;


		/* Form print*/
		// i
		Output->Text += "i = " + k.ToString() + System::Environment::NewLine;
		// hessian
		Output->Text += "Hessian =" + System::Environment::NewLine + "[" + System::Environment::NewLine;
		for (int i = 0; i < H.getRow(); i++) {
			for (int j = 0; j < H.getCol(); j++) {
				Output->Text += H.Data[i].Data[j].ToString() + ", ";
			}
			Output->Text += System::Environment::NewLine;
		}
		Output->Text += "]" + System::Environment::NewLine;
		// hessian inverse
		Output->Text += "Hessian Inverse =" + System::Environment::NewLine + "[" + System::Environment::NewLine;
		for (int i = 0; i < InvH.getRow(); i++) {
			for (int j = 0; j < InvH.getCol(); j++) {
				Output->Text += InvH.Data[i].Data[j].ToString() + ", ";
			}
			Output->Text += System::Environment::NewLine;
		}
		Output->Text += "]" + System::Environment::NewLine;
		// x
		Output->Text += "X =" + System::Environment::NewLine + "[" + System::Environment::NewLine;
		for (int i = 0; i < now_x.getDim(); i++)
			Output->Text += now_x.Data[i].ToString() + System::Environment::NewLine;
		Output->Text += " ]" + System::Environment::NewLine + System::Environment::NewLine;
	}

	Output->Text += "[";
	for (int i = 0; i < pre_x.getDim(); i++) {
		Output->Text += pre_x.Data[i].ToString() + " , ";
	}
	Output->Text += "]" + System::Environment::NewLine;
	Output->Text += System::Environment::NewLine + "min = " + f(pre_x).ToString() + System::Environment::NewLine + System::Environment::NewLine;
}

void Equation::Quasi_Newton(double x, double y, double xMin, double xMax, double yMin, double yMax, System::Windows::Forms::TextBox ^Output)
{
	double Precision = 0.0001;
	int Max_iter = 500;

	// Initial X Vector and Internal Vector
	Vector Max_X, Min_X;
	if (dim == 3) {
		X.initial(x, y);
		Max_X.initial(xMax, yMax);
		Min_X.initial(xMin, yMin);
	}
	else if (dim == 2) {
		X.initial(y);
		Max_X.initial(yMax);
		Min_X.initial(yMin);
	}
	else if (dim == 1) {
		X.initial(x);
		Max_X.initial(xMax);
		Min_X.initial(xMin);
	}

	Vector pre_x = X, now_x = X;
	Matrix Fake_InvH;	// Fake Inverse Hessian
	Fake_InvH.identity(X);	// Initial Identity Matrix
	Vector step;

	int k = 0;
	while (Max_iter-- > 0) {
		pre_x = now_x;
		Matrix G = Gradient(pre_x);
		Vector gradient = G.Data[0];	// Matrix to Vector

		// stop criteria
		if (Norm(gradient) == 0) {
			break;
		}
		Matrix D = G * Transpose(Fake_InvH);
		D = -1 * D;

		// Compute Afa Step (skip)
		Matrix Hess = Hessian(now_x);
		Matrix Afa = -1 * G * Transpose(D) * Inverse(D * Hess * Transpose(D));
		step = Afa.Data[0] * D.Data[0];

		//pre_x = now_x;
		now_x = pre_x + step;

		// Assure Hessian
		while (f(now_x) > f(pre_x)) {
			Matrix I;
			I.identity(now_x);
			Hess = Hess + I;

			// Compute Afa Step (skip)
			Afa = -1 * G * Transpose(D) * Inverse(D * Hess * Transpose(D));
			step = Afa.Data[0] * D.Data[0];

			//pre_x = now_x;
			now_x = pre_x + step;
		}

		// Edge Dealing
		for (int i = 0; i < now_x.getDim(); i++) {
			while (now_x.Data[i] > Max_X.Data[i] || now_x.Data[i] < Min_X.Data[i]) {
				step = 0.9 * step;
				now_x = pre_x - step;
			}
		}

		// Compute Fake Inverse Hessian
		Matrix Step(step);
		Matrix now_G = Gradient(now_x);

		// Not A Number
		while (isnan(now_G.Data[0].Data[0])) {
			step = 0.9 * step;
			now_x = pre_x + step;

			now_G = Gradient(now_x);
		}

		Matrix Yk = now_G - G;
		Matrix M1 = Transpose(Step) * Step;
		double d1 = 1 / (Step * Transpose(Yk)).Data[0].Data[0];

		Matrix M21 = Fake_InvH * Transpose(Yk);
		Matrix M22 = Transpose(Fake_InvH * Transpose(Yk));
		Matrix M2 = M21 * M22;
		double d2 = 1 / (Yk * Fake_InvH * Transpose(Yk)).Data[0].Data[0];

		Fake_InvH = Fake_InvH + d1 * M1 - d2 * M2;


		//Stopping Criteria
		if (abs(Norm(now_x - pre_x)) <= Precision) {
			break;
		}
		k++;


		/* Form print*/
		// i
		Output->Text += "i = " + k.ToString() + System::Environment::NewLine;
		// x
		Output->Text += "X =" + System::Environment::NewLine + "[" + System::Environment::NewLine;
		for (int i = 0; i < now_x.getDim(); i++)
			Output->Text += now_x.Data[i].ToString() + System::Environment::NewLine;
		Output->Text += " ]" + System::Environment::NewLine + System::Environment::NewLine;
		// hessian
		Output->Text += "Hessian Inverse=" + System::Environment::NewLine + "[" + System::Environment::NewLine;
		for (int i = 0; i < Fake_InvH.getRow(); i++) {
			for (int j = 0; j < Fake_InvH.getCol(); j++) {
				Output->Text += Fake_InvH.Data[i].Data[j].ToString() + ", ";
			}
			Output->Text += System::Environment::NewLine;
		}
		Output->Text += "]" + System::Environment::NewLine;
	}
	Output->Text += "[";
	for (int i = 0; i < pre_x.getDim(); i++) {
		Output->Text += pre_x.Data[i].ToString() + " , ";
	}
	Output->Text += "]" + System::Environment::NewLine;
	Output->Text += System::Environment::NewLine + "min = " + f(pre_x).ToString() + System::Environment::NewLine + System::Environment::NewLine;
}

