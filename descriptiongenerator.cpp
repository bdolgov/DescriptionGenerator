#include "descriptiongenerator.hpp"

#include <QWebView>

DescriptionGenerator::DescriptionGenerator(QWidget *parent)
: QWidget(parent)
{
	all = new QVBoxLayout();

	QHBoxLayout *pname = new QHBoxLayout;

	pname->addWidget(new QLabel("Problem name:"));
	pname->addWidget(name = new QLineEdit);

	pname->addWidget(new QLabel("Input file:"));
	pname->addWidget(infile = new QLineEdit);

	pname->addWidget(new QLabel("Output file:"));
	pname->addWidget(outfile = new QLineEdit);

	pname->addWidget(new QLabel("TL:"));
	pname->addWidget(tl = new QLineEdit);

	pname->addWidget(new QLabel("ML:"));
	pname->addWidget(ml = new QLineEdit);

	all->addLayout(pname);

	all->addWidget(new QLabel("Description:"));
	all->addWidget(description = new QTextEdit);

	all->addWidget(new QLabel("Input format:"));
	all->addWidget(input = new QTextEdit);

	all->addWidget(new QLabel("Output format:"));
	all->addWidget(output = new QTextEdit);

	for (int i = 0; i < TESTS; ++i)
	{
		all->addWidget(new QLabel(QString("Test %1:").arg(i + 1)));
		QHBoxLayout *l = new QHBoxLayout;
		tests[i][0] = new QTextEdit;
		tests[i][1] = new QTextEdit;
		l->addWidget(tests[i][0]);
		l->addWidget(tests[i][1]);
		all->addLayout(l);
	}


	go = new QPushButton("Generate!");
	connect(go, SIGNAL(clicked()), this, SLOT(process()));
	all->addWidget(go);
	setLayout(all);

	QFile f1("description.html");
	f1.open(QIODevice::ReadOnly | QIODevice::Text);
	descr = QString::fromUtf8(f1.readAll());
	QFile f2("tests.html");
	f2.open(QIODevice::ReadOnly | QIODevice::Text);
	test = QString::fromUtf8(f2.readAll());
}

DescriptionGenerator::~DescriptionGenerator()
{
	
}
void DescriptionGenerator::process()
{
	QString result = descr;
	QString tests_result;
	for (int i = 0; i < TESTS && !tests[i][0]->toPlainText().isEmpty(); ++i)
	{
		QString t = test;
		t.replace("__IN__", parseRaw(tests[i][0]->toPlainText()));
		t.replace("__OUT__", parseRaw(tests[i][1]->toPlainText()));
		tests_result.append(t);
	}
	result.replace("__TESTS__", tests_result);
	result.replace("__PROBLEM__", name->text());
	result.replace("__INFILE__", infile->text());
	result.replace("__OUTFILE__", outfile->text());
	result.replace("__TL__", tl->text());
	result.replace("__ML__", ml->text());
	result.replace("__DESCRIPTION__", parseHtml(description->toPlainText()));
	result.replace("__INPUT__", parseHtml(input->toPlainText()));
	result.replace("__OUTPUT__", parseHtml(output->toPlainText()));

	DescriptionPreview *p = new DescriptionPreview(result);
	p->show();
}

QString DescriptionGenerator::parseRaw(QString s)
{
	s.replace(">", "&gt");
	s.replace("<", "&lt");
	s.replace("\"", "&quot;");
	return s;
}

QString DescriptionGenerator::parseHtml(QString s)
{
	QString ret;
	char state = 0;
	//s.replace("&", "&amp; ");
	s.replace(" >= ", " &ge; ");
	s.replace(" <= ", " &le; ");
	s.replace(" < ", " &lt; ");
	s.replace(" > ", " &gt; ");
	s.prepend("<p>");
	s.replace("\n\n", "</p><p>");
	s.append("</p>");
	for (QString::iterator i = s.begin(); i != s.end(); ++i)
	{
		if (state == 0 && (*i == '_' || *i == '^'))
		{
			ret.append(*i == '_' ? "<span class=\"sub\">" : "<span class=\"sup\">");
			++i;
			if (*i != '{')
			{
				ret.append(*i);
				ret.append("</span>");
			}
			else
			{
				state = '_';
			}
		}
		else if (state == '_' && *i == '}')
		{
			ret.append("</span>");
			state = 0;
		}
		else if (state == 0 && *i == '<')
		{
			state = '<';
			ret.append('<');
		}
		else if (state != '<' && *i == '"')
		{
			ret.append("&quot;");
		}
		else if (state =='<' && *i == '>')
		{
			state = 0;
			ret.append('>');
		}
		else
		{
			ret.append(*i);
		}
	}
	return ret;
}

DescriptionPreview::DescriptionPreview(QString _code, QWidget *parent): QWidget(parent)
{
	code = _code;
	QVBoxLayout *v = new QVBoxLayout;
	setWindowTitle("Preview");
	QWebView *w = new QWebView;
	v->addWidget(w);
	QPushButton *b = new QPushButton("Save!");
	v->addWidget(b);
	connect(b, SIGNAL(clicked()), this, SLOT(save()));
	w->setHtml(code);
	setLayout(v);
	setAttribute(Qt::WA_DeleteOnClose);
}

void DescriptionPreview::save()
{
	QString path = QFileDialog::getSaveFileName(this, "Select path");
	QFile f(path);
	f.open(QIODevice::WriteOnly | QIODevice::Text);
	f.write(code.toUtf8());
	f.flush();
	f.close();
}
