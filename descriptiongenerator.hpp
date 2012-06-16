#ifndef DESCRIPTIONGENERATOR_HPP
#define DESCRIPTIONGENERATOR_HPP

#include <QtGui/QMainWindow>
#include <QtGui>
#define TESTS 3
class DescriptionGenerator : public QWidget
{
		Q_OBJECT
		
	public:
		DescriptionGenerator(QWidget *parent = 0);
		~DescriptionGenerator();

	private:
		QVBoxLayout *all;
		QLineEdit *name, *infile, *outfile, *tl, *ml;
		QTextEdit *description, *input, *output;
		QPushButton *go;
		QTextEdit *tests[TESTS][2];

		QString descr, test;

		QString parseRaw(QString s);
		QString parseHtml(QString s);

	private slots:
		void process();
};

class DescriptionPreview : public QWidget
{
		Q_OBJECT

	public:
		DescriptionPreview(QString _code, QWidget *parent = 0);
	private slots:
		void save();
	private:
		QString code;
	protected:
		void closeEvent(QCloseEvent *);
};

#endif // DESCRIPTIONGENERATOR_HPP
