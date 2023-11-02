
#ifndef NEURALNETWORKWRAPER_H
#define NEURALNETWORKWRAPER_H


#include<Neural_Network.hpp>


#include <QObject>
#include <string>
#include <QDir>
#include <QCoreApplication>


/*
 * PerfectSet - 0
 * GoodSet - 1
 * NormSet - 2
 * BadSet - 3
 * WithoutStudying - 4
*/
class NeuralNetworkWraper : public QObject
{
    Q_OBJECT
    constexpr static int h = 28, w = 28;

    std::string path[5];

public:
    explicit NeuralNetworkWraper(QObject *parent = nullptr);
    NeuralNetwork<h* w, 16, 16, 10> network;

signals:
    void updateAnswer(int answer);

    void updateNeurons(QList<QList<double>> weights);

    void updateNeuronsColor(int colorIndex);

public slots:
    void answer(const QList<bool> &list, int layerIndex);
    void setSet(int index);

};

#endif // NEURALNETWORKWRAPER_H
