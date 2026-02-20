#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

class Caseta {
private:
    int x1, y1, x2, y2;
    std::string eticheta;

public:
    Caseta(int x1, int y1, int x2, int y2, std::string eticheta) 
        : x1(x1), y1(y1), x2(x2), y2(y2), eticheta(eticheta) {}

    void afisare() const {
        std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
        std::cout << eticheta << std::endl;
    }

    std::string getEticheta() const { 
        return eticheta; 
    }

    double getSuprafata() const {
        double latime = (double)x2 - x1 + 1;
        double inaltime = (double)y2 - y1 + 1;
        if (latime <= 0 || inaltime <= 0) return 0;
        return latime * inaltime;
    }

    static double calculeazaIoU(const Caseta& A, const Caseta& B) {
        int cx1 = std::max(A.x1, B.x1);
        int cx2 = std::min(A.x2, B.x2);
        int cy1 = std::max(A.y1, B.y1);
        int cy2 = std::min(A.y2, B.y2);

        double suprafataIntersectie = 0;
        if (cx1 <= cx2 && cy1 <= cy2) {
            suprafataIntersectie = (double)(cx2 - cx1 + 1) * (cy2 - cy1 + 1);
        }

        double suprafataReuniune = A.getSuprafata() + B.getSuprafata() - suprafataIntersectie;
        
        if (suprafataReuniune <= 0) return 0;
        return suprafataIntersectie / suprafataReuniune;
    }
};

int main() {
    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<Caseta*> referinta;
    for (int i = 0; i < n; i++) {
        int x1, y1, x2, y2;
        std::string label;
        std::cin >> x1 >> y1 >> x2 >> y2 >> label;
        referinta.push_back(new Caseta(x1, y1, x2, y2, label));
    }

    int operatie;
    std::cin >> operatie;

    if (operatie == 1) {
        for (int i = 0; i < n; i++) {
            referinta[i]->afisare();
        }
    } 
    else if (operatie == 2) {
        int x1, y1, x2, y2;
        std::string label;
        std::cin >> x1 >> y1 >> x2 >> y2 >> label;
        Caseta ipoteza(x1, y1, x2, y2, label);
        double iou = Caseta::calculeazaIoU(*referinta[0], ipoteza);
        std::cout << std::fixed << std::setprecision(4) << iou << std::endl;
    } 
    else if (operatie == 3) {
        int x1, y1, x2, y2;
        std::string label;
        double prag;
        std::cin >> x1 >> y1 >> x2 >> y2 >> label >> prag;
        Caseta ipoteza(x1, y1, x2, y2, label);
        double iou = Caseta::calculeazaIoU(*referinta[0], ipoteza);
        if (referinta[0]->getEticheta() == label && iou > prag) {
            std::cout << 1 << std::endl;
        } else {
            std::cout << 0 << std::endl;
        }
    } 
    else if (operatie == 4) {
        int m;
        std::cin >> m;
        std::vector<Caseta*> ipoteze;
        for (int i = 0; i < m; i++) {
            int x1, y1, x2, y2;
            std::string label;
            std::cin >> x1 >> y1 >> x2 >> y2 >> label;
            ipoteze.push_back(new Caseta(x1, y1, x2, y2, label));
        }
        double prag;
        std::cin >> prag;

        int tp = 0;
        int limit = std::min((int)referinta.size(), m);
        for (int i = 0; i < limit; i++) {
            double iou = Caseta::calculeazaIoU(*referinta[i], *ipoteze[i]);
            if (referinta[i]->getEticheta() == ipoteze[i]->getEticheta() && iou > prag) {
                tp++;
            }
        }
        double precizie = (double)tp / m;
        std::cout << std::fixed << std::setprecision(4) << precizie << std::endl;

        for (auto p : ipoteze) delete p;
    }

    for (auto p : referinta) delete p;
    return 0;
}