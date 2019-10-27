#include "interpolation.h"

Interpolation::Interpolation() : m_n(0), m_vertexList(NULL), m_f(NULL)
{
}

Interpolation::~Interpolation()
{
    delete m_f;
    m_f = 0;
}

int Interpolation::Factorial(int n)
{
    int result = 1;
    for (int i = 2; i <= n; ++i)
    {
        result *= i;
    }
    return result;
}

double Interpolation::F(double x)
{
    long double result = m_f[0];
    long double diff = 1.0f;

    for (int k = 1; k < m_n; ++k)
    {
        diff *= x - m_vertexList[k - 1]->pos.x;
        result += diff * m_f[k];
    }

    return result;
}

void Interpolation::Calculate()
{
    int size = m_vertexList.size();
    if (size > 0)
    {
        if (m_f)
        {
            delete[] m_f;
            m_f = 0;
        }

        m_f = new double[size];

        int tmp;
        int i, k;
        long double diff;

        int* index = new int[m_n];
        double* second =
            new double[m_n]; // second, temporary, table for x - value

        /*i = 0;
        for (auto obj : m_vertexList)
        {
            second[i] = obj->position.x;
            ++i;
        }*/

        tmp = 0;
        index[0] = 0;
        second[0] = (m_vertexList)[0]->pos.x;
        m_f[0] = (m_vertexList[0])->pos.y;

        for (i = 1; i < m_n; ++i)
        {
            if (m_vertexList[i]->pos.x != m_vertexList[i - 1]->pos.x)
            {
                tmp = i;
            }
            index[i] = tmp;
            second[i] = m_vertexList[i]->pos.x;
            m_f[i] = m_vertexList[tmp]->pos.y;
        }

        for (i = 1; i < m_n; ++i)
        {
            for (k = m_n - 1; k >= i; --k)
            {
                diff = m_vertexList[k]->pos.x - second[k - 1];

                if (diff == 0)
                {
                    m_f[k] =
                        (m_vertexList[index[k] + i]->pos.y) / Factorial(i);
                }
                else
                {
                    second[k] = second[k - 1];
                    m_f[k] = (m_f[k] - m_f[k - 1]) / diff;
                }
            }
        }
        delete[] index;
        delete[] second;
    }
    return;
}

void Interpolation::AddPoint(float x, float y, float z)
{
    VertexType* vertex = new VertexType;
    vertex->pos.x = x;
    vertex->pos.y = y;
    vertex->pos.z = z;

    m_vertexList.push_back(vertex);

    std::sort(m_vertexList.begin(),
              m_vertexList.end(),
              [](VertexType* v1, VertexType* v2) -> bool {
                  return (
                      (v1->pos.x < v2->pos.x) ||
                      ((v1->pos.x == v2->pos.x) && (v1->pos.z < v2->pos.z)));
              });

    ++m_n;
}

void Interpolation::DeletePoint(int nr)
{
    m_vertexList.erase(m_vertexList.begin() + nr);
    --m_n;
}

Interpolation* Interpolation::Get()
{
    return this;
}

DirectX::XMFLOAT3 Interpolation::GetPoint(int index) const
{
    int size = m_vertexList.size();
    if (index >= size)
    {
        index = size - 1;
    }

    return m_vertexList[index]->pos;
}

int Interpolation::GetListCount() const
{
    return m_vertexList.size();
}
