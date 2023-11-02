
#include "neuralnetworkwraper.h"

NeuralNetworkWraper::NeuralNetworkWraper(QObject *parent)
    : QObject{parent}
{
    QDir dir(QCoreApplication::applicationDirPath());
    path[0] = dir.absolutePath().toStdString()+ QString(QDir::separator()).toStdString() + "GoodSetPerfectStudying.txt";
    path[1] = dir.absolutePath().toStdString()+ QString(QDir::separator()).toStdString() + "GoodSet(50,10, 0 0001).txt";
    path[2] = dir.absolutePath().toStdString()+ QString(QDir::separator()).toStdString() + "GoodSet(50,1, 0 0001).txt";
    path[3] = dir.absolutePath().toStdString()+ QString(QDir::separator()).toStdString() + "BadSet(30,500,0 0001).txt";
    path[4] = dir.absolutePath().toStdString()+ QString(QDir::separator()).toStdString() + "WithoutStudying.txt";
}

void NeuralNetworkWraper::answer(const QList<bool> &list, int layerIndex)
{
    int dimension = sqrt(list.length());
    int scalar = h/dimension;
    std::vector<double> data(h*w);
    for(int i = 0; i<dimension;i++)
    {
        for(int j = 0; j<dimension;j++)
            //data[(i*scalar+1) *w +j*scalar+1] = data[(i*scalar+1) *w +j*scalar] = data[i*scalar *w +j*scalar+1] = data[i*scalar *w +j*scalar] = list.at(i*dimension+j);
            data[i*w+j] = list.at(i*w+j);
    }
    int answer = network.GetAnswer(data);

    QList<QList<double>> arr;
    int neurons[] {16,16,10};

    network.m_layers[3].m_neuronsAfterSigmoida = network.ReLU(network.m_layers[3].m_neuronsBeforeSigmoida);

    for(int i = 0;i<4;i++)
        network.m_layers[i].m_neuronsAfterSigmoida = network.Sigmoid(network.m_layers[i].m_neuronsAfterSigmoida);


    for(int i = 0;i<3;i++)
    {
        QList<double> temp;
        for(int j = 0; j< neurons[i] ;j++)
            temp<<network.m_layers[i+1].m_neuronsAfterSigmoida[j][0];
        arr<<temp;
    }

    emit updateNeurons(arr);


    if(layerIndex == 3)
        emit updateAnswer(answer);
}

void NeuralNetworkWraper::setSet(int index)
{
    network.ReadWeightsAndBayeses(path[index]);
    emit updateNeuronsColor(index);
}

