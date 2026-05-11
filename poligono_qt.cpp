#include <QtWidgets/QApplication>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QPolygonF>
#include <QtCore/QString>
#include <cmath>

// Questo widget si occupa solo del disegno del poligono.
// Riceve numero di lati e lunghezza lato dal main.
class PolygonWidget : public QWidget {
public:
    // Costruttore: salva i dati necessari e imposta una dimensione minima.
    PolygonWidget(int nLati, double lato, QWidget *parent = nullptr)
        : QWidget(parent), n_lati(nLati), lato_lunghezza(lato) {
        setMinimumSize(600, 450);
    }

protected:
    // Qt chiama automaticamente paintEvent quando la finestra va ridisegnata.
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);

        // Oggetto che disegna all'interno del widget.
        QPainter painter(this);
        // Attiva antialiasing per bordi piu' morbidi.
        painter.setRenderHint(QPainter::Antialiasing, true);
        // Colore di sfondo della zona di disegno.
        painter.fillRect(rect(), QColor(20, 20, 28));

        const double pi = 3.14159265358979323846;
        // Formula del raggio circoscritto:
        // R = lato / (2 * sin(pi / n))
        // Serve per ricavare le coordinate dei vertici a partire dal lato.
        const double raggio = lato_lunghezza / (2.0 * std::sin(pi / n_lati));

        // Sceglie un raggio massimo in pixel, proporzionale alla finestra.
        const double maxRaggioPixels = std::min(width(), height()) * 0.35;
        // Fattore di scala "unita' geometriche -> pixel".
        const double scala = maxRaggioPixels / raggio;
        // Centro del widget, usato come centro del poligono.
        const QPointF centro(width() / 2.0, height() / 2.0);

        QPolygonF poly;
        // Evita riallocazioni mentre aggiungiamo i punti.
        poly.reserve(n_lati);
        // Genera i vertici con passo angolare costante:
        // angolo_i = -pi/2 + i * (2*pi / n)
        // -pi/2 mette il primo vertice in alto.
        for (int i = 0; i < n_lati; ++i) {
            const double angolo = -pi / 2.0 + i * (2.0 * pi / n_lati);
            const double x = centro.x() + (raggio * scala) * std::cos(angolo);
            const double y = centro.y() + (raggio * scala) * std::sin(angolo);
            poly << QPointF(x, y);
        }

        // Penna = bordo; brush = riempimento semitrasparente.
        painter.setPen(QPen(QColor(80, 180, 255), 3));
        painter.setBrush(QColor(80, 180, 255, 60));
        painter.drawPolygon(poly);
    }

private:
    int n_lati;
    double lato_lunghezza;
};

int main(int argc, char *argv[]) {
    // Ogni app Qt deve creare QApplication una volta.
    QApplication app(argc, argv);

    // "ok" diventa false se l'utente annulla il dialog.
    bool ok = false;
    // Input numero lati:
    // default=5, min=3, max=1000, step=1.
    int n_lati = QInputDialog::getInt(
        nullptr,
        "Input",
        "Inserisci il numero di lati (>= 3):",
        5,
        3,
        1000,
        1,
        &ok
    );
    if (!ok) {
        // Uscita pulita se l'utente annulla.
        return 0;
    }

    // Input lunghezza lato:
    // default=10.0, min positivo, precisione 4 decimali.
    double lato = QInputDialog::getDouble(
        nullptr,
        "Input",
        "Inserisci la lunghezza del lato (> 0):",
        10.0,
        0.000001,
        1e9,
        4,
        &ok
    );
    if (!ok) {
        return 0;
    }

    const double pi = 3.14159265358979323846;
    // Formule geometriche:
    // Perimetro = n * lato
    // Area = (n * lato^2) / (4 * tan(pi / n))
    const double perimetro = n_lati * lato;
    const double area = (n_lati * lato * lato) / (4.0 * std::tan(pi / n_lati));

    // Finestra principale che contiene label + canvas di disegno.
    QWidget window;
    window.setWindowTitle("Poligono regolare - Qt");

    // Layout verticale: prima info testuale, poi area grafica.
    QVBoxLayout *layout = new QVBoxLayout(&window);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    QLabel *info = new QLabel(
        QString("Lati: %1   Lato: %2   Perimetro: %3   Area: %4")
            .arg(n_lati)
            .arg(lato, 0, 'f', 4)
            .arg(perimetro, 0, 'f', 4)
            .arg(area, 0, 'f', 4)
    );
    info->setStyleSheet("color: #dbeafe; font-size: 14px;");

    // Widget che effettivamente disegna il poligono.
    PolygonWidget *drawArea = new PolygonWidget(n_lati, lato);

    layout->addWidget(info);
    // Stretch=1: quest'area occupa lo spazio verticale rimanente.
    layout->addWidget(drawArea, 1);

    window.resize(820, 620);
    window.show();

    // Avvia il loop eventi Qt (la finestra resta viva finche' non chiudi).
    return app.exec();
}
