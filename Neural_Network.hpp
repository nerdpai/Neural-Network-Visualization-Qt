#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP

#include<cmath>
#include<exception>
#include"Matrix.hpp"
#include<random>
#include<limits>
#include<algorithm>
#include<fstream>


template<class Type>
Matrix_2d<Type> SoftMax(const Matrix_2d<Type>& matrix) //function to get probability
{
	const int row_count = matrix.GetCountOfRows();
	const int col_count = matrix.GetCountOfColumns();
	Matrix_2d<Type> temp(row_count,col_count);
	Type sum{};
	for (int i = 0; i < row_count; i++)
	{
		for(int j = 0;j<col_count;j++)
			sum += std::exp(matrix[i][j]);
	}

	for (int i = 0; i < row_count; i++)
	{
		for (int j = 0; j < col_count; j++)
			temp[i][j] = std::exp(matrix[i][j]) / sum;
	}
	return temp;
}



template<int... NeuronsInLayer> //how many neurons for each layer
class NeuralNetwork
{
	static constexpr int M_LAYERS_COUNT = sizeof...(NeuronsInLayer);

	class NeuralException : public std::exception { public: NeuralException(char const* const _Message) : std::exception(_Message) {} };

	static double Random()
	{
		/*static std::mt19937 rEng(std::random_device{}());
		static std::uniform_real_distribution<double> first_destribution(0.1, 3.0);
		static std::uniform_real_distribution<double> second_destribution(-3.0, -0.1);
		static std::uniform_int_distribution<int> int_destribution(1, 2);
		if(int_destribution(rEng) == 1)
			return first_destribution(rEng);
		if (int_destribution(rEng) == 2)
			return second_destribution(rEng);*/

		static std::default_random_engine rEng(std::random_device{}());
		constexpr auto firstLayer = *(std::initializer_list<int>{ NeuronsInLayer... }.begin());
		static std::normal_distribution<double> destribution(0.0, 1.0/(firstLayer/500.0));
		return destribution(rEng);
	}

	class Layer
	{
		using Matrix = Matrix_2d<double>;

	public:
		Matrix m_weights;
		Matrix m_bayeses;
		Matrix m_neuronsBeforeSigmoida;
		Matrix m_neuronsAfterSigmoida;


		Layer(const int neuronsInMyLayer, const int neuronsInPreviousLayer) : m_weights(neuronsInPreviousLayer == 0 ? 1 : neuronsInMyLayer, neuronsInPreviousLayer == 0 ? 1 : neuronsInPreviousLayer),
			m_bayeses( neuronsInPreviousLayer==0? 1 : neuronsInMyLayer,1),
			m_neuronsBeforeSigmoida(neuronsInMyLayer,1,0.0), m_neuronsAfterSigmoida(neuronsInMyLayer, 1, 0.0)
		{
			m_weights.Random(Random);
			m_bayeses.Random(Random);
		}

		Matrix operator*(const Layer& right) const
		{
			return right.m_weights * m_neuronsAfterSigmoida + right.m_bayeses;
		}

		Layer& operator=(const Matrix& matrix)
		{
			m_neuronsBeforeSigmoida = matrix;
			return *this;
		}
	};

	Matrix_2d<double> ReLU_Deriv(const Matrix_2d<double>& matrix)
	{
		const int row_count = matrix.GetCountOfRows();
		const int col_count = matrix.GetCountOfColumns();
		Matrix_2d<double> temp(row_count, col_count);

		if (col_count != 1)
			throw NeuralException("its not a vector");

		for (int i = 0; i < matrix.GetCountOfRows(); i++)
			temp[i][0] = matrix[i][0] < 0 ? 0 : 1;
		return temp;
	}

	double m_sigmoidScalar;

	Matrix_2d<double> Sigmoid_Deriv(const Matrix_2d<double>& matrix)
	{
		const int row_count = matrix.GetCountOfRows();
		const int col_count = matrix.GetCountOfColumns();
		Matrix_2d<double> temp(row_count, col_count);

		if (col_count != 1)
			throw NeuralException("its not a vector");

		for (int i = 0; i < matrix.GetCountOfRows(); i++)
			temp[i][0] = -m_sigmoidScalar * std::exp(m_sigmoidScalar * matrix[i][0]) / std::pow((1 + std::exp(m_sigmoidScalar * matrix[i][0])), 2);
		return temp;
	}


	std::vector<int> m_countOfNeurons;

	//Matrix_2d<double> m_error;

	int LastElementOfPack() //i don't know why i wrote that
	{
		std::initializer_list<int> init{ NeuronsInLayer... };
		auto data = init.begin();
		for (int i = 1; i < M_LAYERS_COUNT; i++)
			++data;
		return *data;
	}



	void SetNeurons(const std::vector<double>& data)
	{
		for (int i = 0, end = m_countOfNeurons[0]; i < end; i++)
			m_layers[0].m_neuronsAfterSigmoida[i][0] = data[i];

		for (int i = 0; i < M_LAYERS_COUNT - 2; i++)
		{
			m_layers[i + 1] = m_layers[i] * m_layers[i + 1];
			//m_layers[i + 1].m_neuronsAfterSigmoida = Sigmoid(m_layers[i + 1].m_neuronsBeforeSigmoida);
			m_layers[i + 1].m_neuronsAfterSigmoida = ReLU(m_layers[i + 1].m_neuronsBeforeSigmoida);
		}

		m_layers[M_LAYERS_COUNT - 1] = m_layers[M_LAYERS_COUNT - 2] * m_layers[M_LAYERS_COUNT - 1];
	}

	int GetMaxProbability(const Matrix_2d<double>& matrix)
	{
		double temp = std::numeric_limits<double>::min();
		int index = 0;
		for (int i = 0; i < matrix.GetCountOfRows(); i++)
		{
			if(temp<=matrix[i][0])
			{
				temp = matrix[i][0]; index = i;
			}
		}
		return index;
	}


	//study parametrs
	int s_banch;
	int s_epoch;
	double s_step;

	//gradient parametrs
	std::vector<Layer> g_currentGrudient;
	std::vector<Layer> g_banchGradient;


	void BanchGradientToZero()
	{
		for (int i = 1; i < M_LAYERS_COUNT; i++)
		{
			g_banchGradient[i].m_weights.ToZero();
			g_banchGradient[i].m_bayeses.ToZero();
		}
	}



	void SetCurrentGradient(const std::vector<double>& dataForNeurons,int answer)
	{
		SetNeurons(dataForNeurons);
		Matrix_2d<double> correct(m_countOfNeurons[M_LAYERS_COUNT - 1], 1, 0.0);
		correct[answer][0] = 1.0;
		g_currentGrudient[M_LAYERS_COUNT - 1].m_bayeses = SoftMax(m_layers[M_LAYERS_COUNT - 1].m_neuronsBeforeSigmoida) - correct;
		g_currentGrudient[M_LAYERS_COUNT - 1].m_weights = g_currentGrudient[M_LAYERS_COUNT - 1].m_bayeses * m_layers[M_LAYERS_COUNT - 2].m_neuronsAfterSigmoida.T();
		for (int i = M_LAYERS_COUNT - 2; i > 0; --i)
		{
			g_currentGrudient[i].m_bayeses = m_layers[i+1].m_weights.T() * g_currentGrudient[i + 1].m_bayeses; // after Sigmoid
			//g_currentGrudient[i].m_bayeses = g_currentGrudient[i].m_bayeses % Sigmoid_Deriv(m_layers[i].m_neuronsBeforeSigmoida);  //before Sigmoid
			g_currentGrudient[i].m_bayeses = g_currentGrudient[i].m_bayeses % ReLU_Deriv(m_layers[i].m_neuronsBeforeSigmoida);
			g_currentGrudient[i].m_weights = g_currentGrudient[i].m_bayeses * m_layers[i - 1].m_neuronsAfterSigmoida.T();
		}
	}

	std::vector<Layer>& SumWithWrite(std::vector<Layer>& left, const std::vector<Layer>& right)
	{
		for (int i = 1; i < M_LAYERS_COUNT; i++)
		{
			left[i].m_weights += right[i].m_weights;
			left[i].m_bayeses += right[i].m_bayeses;
		}
		return left;
	}

	void SetNewWeights()
	{
		for (int i = 1; i < M_LAYERS_COUNT; i++)
		{
			m_layers[i].m_weights -= g_banchGradient[i].m_weights * s_step;
			m_layers[i].m_bayeses -= g_banchGradient[i].m_bayeses * s_step;
		}
	}

public:
    std::vector<Layer> m_layers;

    Matrix_2d<double> ReLU(const Matrix_2d<double>& matrix)
    {
        const int row_count = matrix.GetCountOfRows();
        const int col_count = matrix.GetCountOfColumns();
        Matrix_2d<double> temp(row_count, col_count);

        if (col_count != 1)
            throw NeuralException("its not a vector");

        for (int i = 0; i < matrix.GetCountOfRows(); i++)
            temp[i][0] = matrix[i][0] < 0 ? 0 : matrix[i][0];
        return temp;
    }

    Matrix_2d<double> Sigmoid(const Matrix_2d<double>& matrix)
    {
        const int row_count = matrix.GetCountOfRows();
        const int col_count = matrix.GetCountOfColumns();
        Matrix_2d<double> temp(row_count, col_count);

        if (col_count != 1)
            throw NeuralException("its not a vector");

        for (int i = 0; i < matrix.GetCountOfRows(); i++)
            temp[i][0] = 1.0 / (1.0 + std::exp(m_sigmoidScalar * matrix[i][0]));
        return temp;
    }

	NeuralNetwork(int banch = 1, int epoch = 20, double step = 0.001, double sigmoidScalar = -0.3) : m_sigmoidScalar(sigmoidScalar)// : m_error(LastElementOfPack(),1,0)
	{
		if (M_LAYERS_COUNT < 2)
			throw NeuralException("The count of layers should be 2 or more");

		SetStudyParametrs(banch, epoch, step);

		m_countOfNeurons = std::initializer_list<int>{ NeuronsInLayer... };

		for (int i = 0; i < M_LAYERS_COUNT; i++)
		{
			if (i == 0)
				m_layers.push_back(Layer(m_countOfNeurons[i], 0));
			else
				m_layers.push_back(Layer(m_countOfNeurons[i], m_countOfNeurons[i-1]));
		}

		g_banchGradient = g_currentGrudient = m_layers;
	}

	void SetSigmoidScalar(double scalar) { m_sigmoidScalar = scalar; }

	void SetStudyParametrs(int banch, int epoch, double step)
	{
		if( banch < 1 || epoch < 1 || step <= std::numeric_limits<double>::epsilon())
			throw NeuralException("Wrong study parametrs");
		s_banch = banch;
		s_epoch = epoch;
		s_step = step;
	}


	int GetAnswer(const std::vector<double>& data)
	{
		SetNeurons(data);
		return GetMaxProbability(SoftMax(m_layers[M_LAYERS_COUNT - 1].m_neuronsBeforeSigmoida));
	}



	using StudyData = std::vector<std::vector<double>>;
	void Study(const StudyData& studyNeurons, const std::vector<int>& correctAnswers)
	{
		const auto dataSize = correctAnswers.size();
		if(studyNeurons.size() != dataSize)
			throw NeuralException("Invalid count of arguments");

		for (int i = 0; i < dataSize; i++)
		{
			if(correctAnswers[i]<0 || correctAnswers[i]>= m_countOfNeurons[M_LAYERS_COUNT - 1])
				throw NeuralException("Invalid possible answer");
		}

		using StudyMatrial = std::vector<std::pair<std::vector<double>, int>>;

		StudyMatrial studyData(dataSize);
		for (int i = 0; i < dataSize; i++)
		{
			studyData[i] = std::pair<std::vector<double>, int>(studyNeurons[i], correctAnswers[i]);
		}

		const int banch = s_banch > dataSize ? dataSize : s_banch;
		auto rng = std::default_random_engine{ std::random_device{}() };
		for (int i = 0; i < s_epoch; i++)
		{
			std::shuffle(studyData.begin(), studyData.end(), rng);

			for (int j = 0; j < dataSize; j+= banch)
			{
				BanchGradientToZero();

				const int restElementsInBanch = j + banch > dataSize ? dataSize - j : banch;
				for (int k = 0; k < restElementsInBanch; k++)
				{
					SetCurrentGradient(studyData[j + k].first, studyData[j + k].second);
					SumWithWrite(g_banchGradient, g_currentGrudient);
				}
				SetNewWeights();
			}

		}

	}


	void SaveWeightsAndBayeses(const std::string& path)
	{
		std::ofstream fout(path);
		try
		{
			if (!fout.is_open())
				throw NeuralException("Can't write in file");

			fout << m_countOfNeurons[0];
			for (int i = 1; i < M_LAYERS_COUNT; i++)
				fout << " " << m_countOfNeurons[i];
			fout << "\n";//header

			for (int i = 1; i < M_LAYERS_COUNT;i++) //data
			{
				fout << m_layers[i].m_bayeses[0][0];
				for(int j = 1;j<m_countOfNeurons[i];j++)
					fout <<" "<< m_layers[i].m_bayeses[j][0];
				fout << "\n";//bayeses

				
				const int rows = m_layers[i].m_weights.GetCountOfRows();
				const int cols = m_layers[i].m_weights.GetCountOfColumns();
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
					{
						if (j == 0 && k == 0)
							fout << m_layers[i].m_weights[j][k];
						else
							fout <<" "<< m_layers[i].m_weights[j][k];
					}
				}
				fout << "\n";//weights
			}
			fout.close();
		}
		catch (...)
		{
			fout.close();
			throw;
		}
	}

	void ReadWeightsAndBayeses(const std::string& path)
	{
		std::ifstream fin(path);
		try
		{
			if (!fin.is_open())
				throw NeuralException("Can't read from in file");

			int header;
			for (int i = 0; i < M_LAYERS_COUNT; i++)
			{
				fin >> header;
				if(header!=m_countOfNeurons[i])
					throw NeuralException("Can't read form file");
			}//check header


			for (int i = 1; i < M_LAYERS_COUNT;i++) //data
			{
				for (int j = 0; j < m_countOfNeurons[i]; j++)
					fin >> m_layers[i].m_bayeses[j][0];//bayeses


				const int rows = m_layers[i].m_weights.GetCountOfRows();
				const int cols = m_layers[i].m_weights.GetCountOfColumns();
				for (int j = 0; j < rows; j++)
				{
					for (int k = 0; k < cols; k++)
						fin >> m_layers[i].m_weights[j][k];
				}//weights
			}
			fin.close();
		}
		catch (...)
		{
			fin.close();
			throw;
		}
	}

};

#endif
